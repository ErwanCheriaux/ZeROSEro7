//main.c

#include "ch.h"
#include "hal.h"
#include "usbh/debug.h" /* for usbDbgPuts/uinfof */

#include "fibo.h"

#include "led.h"
#include "rtt.h"
#include "usb.h"
#include "timer.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "usbh_custom_class_example.h"

#if HAL_USBH_USE_HID
#include "usbh/dev/hid.h"
#include "chprintf.h"

static THD_WORKING_AREA(waTestHID, 1024);

static void _hid_report_callback(USBHHIDDriver *hidp, uint16_t len) {
    uint8_t *report = (uint8_t *)hidp->config->report_buffer;

    if (hidp->type == USBHHID_DEVTYPE_BOOT_MOUSE) {
        uinfof("Mouse report: buttons=%02x, Dx=%d, Dy=%d",
                report[0],
                (int8_t)report[1],
                (int8_t)report[2]);
    } else if (hidp->type == USBHHID_DEVTYPE_BOOT_KEYBOARD) {
        uinfof("Keyboard report: modifier=%02x, keys=%02x %02x %02x %02x %02x %02x",
                report[0],
                report[2],
                report[3],
                report[4],
                report[5],
                report[6],
                report[7]);
    } else {
        uinfof("Generic report, %d bytes", len);
    }
}

static USBH_DEFINE_BUFFER(uint8_t report[HAL_USBHHID_MAX_INSTANCES][8]);
static USBHHIDConfig hidcfg[HAL_USBHHID_MAX_INSTANCES];

static void ThreadTestHID(void *p) {
    (void)p;
    uint8_t i;
    static uint8_t kbd_led_states[HAL_USBHHID_MAX_INSTANCES];

    chRegSetThreadName("HID");

    for (i = 0; i < HAL_USBHHID_MAX_INSTANCES; i++) {
        hidcfg[i].cb_report = _hid_report_callback;
        hidcfg[i].protocol = USBHHID_PROTOCOL_BOOT;
        hidcfg[i].report_buffer = report[i];
        hidcfg[i].report_len = 8;
    }

    for (;;) {
        for (i = 0; i < HAL_USBHHID_MAX_INSTANCES; i++) {
            if (usbhhidGetState(&USBHHIDD[i]) == USBHHID_STATE_ACTIVE) {
                uinfof("HID: Connected, HID%d", i);
                usbhhidStart(&USBHHIDD[i], &hidcfg[i]);
                if (usbhhidGetType(&USBHHIDD[i]) != USBHHID_DEVTYPE_GENERIC) {
                    usbhhidSetIdle(&USBHHIDD[i], 0, 0);
                }
                kbd_led_states[i] = 1;
            } else if (usbhhidGetState(&USBHHIDD[i]) == USBHHID_STATE_READY) {
                if (usbhhidGetType(&USBHHIDD[i]) == USBHHID_DEVTYPE_BOOT_KEYBOARD) {
                    USBH_DEFINE_BUFFER(uint8_t val);
                    val = kbd_led_states[i] << 1;
                    if (val == 0x08) {
                        val = 1;
                    }
                    usbhhidSetReport(&USBHHIDD[i], 0, USBHHID_REPORTTYPE_OUTPUT, &val, 1);
                    kbd_led_states[i] = val;
                }
            }
        }
        chThdSleepMilliseconds(200);
    }

}
#endif

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    rtt_init();
    usb_init();
    timer_init();

    timer_on();

    /*USBH OTG*/
//  palSetPadMode(GPIOB, GPIOB_OTG_HS_VBUS, PAL_MODE_INPUT_PULLDOWN);
//  palSetPadMode(GPIOB, GPIOB_OTG_HS_ID, PAL_MODE_ALTERNATE(10));
//  palSetPadMode(GPIOB, GPIOB_OTG_HS_DM, PAL_MODE_ALTERNATE(10));
//  palSetPadMode(GPIOB, GPIOB_OTG_HS_DP, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_USB_HS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
//  palSetPadMode(GPIOF, GPIOF_USB_HS_FAULT, PAL_MODE_INPUT);

#if HAL_USBH_USE_HID
    chThdCreateStatic(waTestHID, sizeof(waTestHID), NORMALPRIO, ThreadTestHID, 0);
#endif

    uinfo("turn on USB power");
    palSetPad(GPIOA, GPIOA_USB_HS_BUSON);
    chThdSleepMilliseconds(100);

    usbhStart(&USBHD2);

    while(1) {
        usbhMainLoop(&USBHD2);
        chThdSleepMilliseconds(100);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
