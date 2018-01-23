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

//extern var
uint8_t  led_status = 7;
uint16_t password[200];
uint16_t password_size = 0;

static THD_WORKING_AREA(waTestHID, 1024);

static void _hid_report_callback(USBHHIDDriver *hidp, uint16_t len)
{
    (void)len;
    uint8_t *report = (uint8_t *)hidp->config->report_buffer;

    //get every input
    static int password_index = 0;
    //    password[password_index] = hid2azerty(report);
    if(password_index++ >= 200)
        password_index = 0;
    if(password_size++ >= 200)
        password_size = 200;

    if(hidp->type == USBHHID_DEVTYPE_BOOT_KEYBOARD) {
        /* send the key on the computer */
        usb_report(&UHD2, report, 8);
        if(report[2] == KEY_F2) {
            static uint16_t msg1 = 0x0500;
            spi_write(&msg1, 1);
        } else if(report[2] == KEY_F3) {
            const int       n = 100;
            static uint16_t msg2[100];
            for(uint16_t i = 0; i < n; i++)
                msg2[i]    = i;
            spi_write(msg2, n);
        } else if(report[2] == KEY_F4) {
            const int       n = 500;
            static uint16_t msg3[500];
            for(uint16_t i = 0; i < n; i++)
                msg3[i]    = i;
            spi_write(msg3, n);
        } else if(report[2] == KEY_F5) {
            spi_display_buffer(10);
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
