/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "hal.h"

#include "rtt.h"
#include "usb.h"
#include "usbh.h"
#include "flash.h"

#include <string.h>

/*
 * USB HID Driver structure.
 */
USBHIDDriver UHD2;

/*
 * Endpoints to be used for USBD2.
 */
#define USBD2_DATA_REQUEST_EP 1
#define USBD2_DATA_AVAILABLE_EP 1

/*
 * USB Device Descriptor.
 *
 * DEVICE DESCRIPTOR
 *  bLength: 18
 *  bDescriptorType: 0x01 (DEVICE)
 *  bcdUSB: 0x0110
 *  bDeviceClass: Device (0x00)
 *  bDeviceSubClass: 0
 *  bDeviceProtocol: 0 (Use class code info from Interface Descriptors)
 *  bMaxPacketSize0: 8
 *  idVendor: Dell Computer Corp. (0x413c)
 *  idProduct: Unknown (0x2107)
 *  bcdDevice: 0x0178
 *  iManufacturer: 1
 *  iProduct: 2
 *  iSerialNumber: 0
 *  bNumConfigurations: 1
 */
static uint8_t hid_device_descriptor_data[18];

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor hid_device_descriptor = {
    sizeof hid_device_descriptor_data,
    hid_device_descriptor_data};

/* Configuration Descriptor DELL Keyboard
 *
 * CONFIGURATION DESCRIPTOR
 *  bLength: 9
 *  bDescriptorType: 0x02 (CONFIGURATION)
 *  wTotalLength: 34
 *  bNumInterfaces: 1
 *  bConfigurationValue: 1
 *  iConfiguration: 0
 *  Configuration bmAttributes: 0xa0  NOT SELF-POWERED  REMOTE-WAKEUP
 *      1... .... = Must be 1: Must be 1 for USB 1.1 and higher
 *      .0.. .... = Self-Powered: This device is powered from the USB bus
 *      ..1. .... = Remote Wakeup: This device supports REMOTE WAKEUP
 *  bMaxPower: 50  (100mA)
 *
 * INTERFACE DESCRIPTOR (0.0): class HID
 *  bLength: 9
 *  bDescriptorType: 0x04 (INTERFACE)
 *  bInterfaceNumber: 0
 *  bAlternateSetting: 0
 *  bNumEndpoints: 1
 *  bInterfaceClass: HID (0x03)
 *  bInterfaceSubClass: Boot Interface (0x01)
 *  bInterfaceProtocol: Keyboard (0x01)
 *  iInterface: 0
 *
 *  HID DESCRIPTOR
 *   bLength: 9
 *   bDescriptorType: 0x21 (HID)
 *   bcdHID: 0x0110
 *   bCountryCode: Not Supported (0x00)
 *   bNumDescriptors: 1
 *   bDescriptorType: HID Report (0x22)
 *   wDescriptorLength: 65
 *
 *  ENDPOINT DESCRIPTOR
 *   bLength: 7
 *   bDescriptorType: 0x05 (ENDPOINT)
 *   bEndpointAddress: 0x81  IN  Endpoint:1
 *       1... .... = Direction: IN Endpoint
 *       .... 0001 = Endpoint Number: 0x1
 *   bmAttributes: 0x03
 *       .... ..11 = Transfertype: Interrupt-Transfer (0x3)
 *   wMaxPacketSize: 8
 *       ...0 0... .... .... = Transactions per microframe: 1 (0)
 *       .... ..00 0000 1000 = Maximum Packet Size: 8
 *   bInterval: 10
 */

static const uint8_t hid_configuration_descriptor_data[34] = {
    /* Configuration Descriptor.*/
    USB_DESC_CONFIGURATION(34,   /* wTotalLength.                    */
                           0x01, /* bNumInterfaces.                  */
                           0x01, /* bConfigurationValue.             */
                           0,    /* iConfiguration.                  */
                           0xA0, /* bmAttributes (self powered).     */
                           50),  /* bMaxPower (100mA).               */
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE(0x00, /* bInterfaceNumber.                */
                       0x00, /* bAlternateSetting.               */
                       0x01, /* bNumEndpoints.                   */
                       0x03, /* bInterfaceClass                  */
                       0x01, /* bInterfaceSubClass               */
                       0x01, /* bInterfaceProtocol               */
                       0),   /* iInterface.                      */
    /* HID Descriptor.*/
    USB_DESC_HID(0x0110, /* bcdHID.                              */
                 0x00,   /* bCountryCode.                        */
                 0x01,   /* bNumDescriptors.                     */
                 0x22,   /* bDescriptorTypeHID                   */
                 65),    /* wDescriptorLength                    */
    /* Endpoint 2 Descriptor.*/
    USB_DESC_ENDPOINT(0x81,
                      0x03, /* bmAttributes (Interrupt).        */
                      8,    /* wMaxPacketSize.                  */
                      10)   /* bInterval.                       */
};

/*
 * Configuration Descriptor wrapper.
 */
static const USBDescriptor hid_configuration_descriptor = {
    sizeof hid_configuration_descriptor_data,
    hid_configuration_descriptor_data};

/*
 * HID Descriptor wrapper.
 */
#define HID_DESCRIPTOR_OFFSET 18
#define HID_DESCRIPTOR_SIZE USB_DESC_HID_SIZE

static const USBDescriptor hid_descriptor = {
    HID_DESCRIPTOR_SIZE,
    &hid_configuration_descriptor_data[HID_DESCRIPTOR_OFFSET]};

/*
 * HID Report Descriptor
 *
 * This is the description of the format and the content of the
 * different IN or/and OUT reports that your application can
 * receive/send
 *
 * See "Device Class Definition for Human Interface Devices (HID)"
 * (http://www.usb.org/developers/hidpage/HID1_11.pdf) for the
 * detailed description of all the fields
 */
static const uint8_t hid_report_descriptor_data[] = {
    USB_DESC_BYTE(0x05), USB_DESC_BYTE(0x01),  // USAGE_PAGE (Generic Desktop)
    USB_DESC_BYTE(0x09), USB_DESC_BYTE(0x06),  // USAGE (Keyboard)
    USB_DESC_BYTE(0xa1), USB_DESC_BYTE(0x01),  // COLLECTION (Application)
                                               // --------------------- input report for modifier keys
    USB_DESC_BYTE(0x05), USB_DESC_BYTE(0x07),  //   USAGE_PAGE (Keyboard)
    USB_DESC_BYTE(0x19), USB_DESC_BYTE(0xe0),  //   USAGE_MINIMUM (Keyboard LeftControl)
    USB_DESC_BYTE(0x29), USB_DESC_BYTE(0xe7),  //   USAGE_MAXIMUM (Keyboard Right GUI)
    USB_DESC_BYTE(0x15), USB_DESC_BYTE(0x00),  //   LOGICAL_MINIMUM (0)
    USB_DESC_BYTE(0x25), USB_DESC_BYTE(0x01),  //   LOGICAL_MAXIMUM (1)
    USB_DESC_BYTE(0x75), USB_DESC_BYTE(0x01),  //   REPORT_SIZE (1)
    USB_DESC_BYTE(0x95), USB_DESC_BYTE(0x08),  //   REPORT_COUNT (8)
    USB_DESC_BYTE(0x81), USB_DESC_BYTE(0x02),  //   INPUT (Data,Var,Abs)
                                               // --------------------- input report - padding
    USB_DESC_BYTE(0x95), USB_DESC_BYTE(0x01),  //   REPORT_COUNT (1)
    USB_DESC_BYTE(0x75), USB_DESC_BYTE(0x08),  //   REPORT_SIZE (8)
    USB_DESC_BYTE(0x81), USB_DESC_BYTE(0x01),  //   INPUT (Cnst,Var,Abs)                         = 0x81, 0x03
                                               // --------------------- output report for LED
    USB_DESC_BYTE(0x95), USB_DESC_BYTE(0x03),  //   REPORT_COUNT (5)                             = 0x95, 0x05
    USB_DESC_BYTE(0x75), USB_DESC_BYTE(0x01),  //   REPORT_SIZE (1)
    USB_DESC_BYTE(0x05), USB_DESC_BYTE(0x08),  //   USAGE_PAGE (LEDs)
    USB_DESC_BYTE(0x19), USB_DESC_BYTE(0x01),  //   USAGE_MINIMUM (Num Lock)
    USB_DESC_BYTE(0x29), USB_DESC_BYTE(0x03),  //   USAGE_MAXIMUM (Kana)                         = 0x29, 0x05
    USB_DESC_BYTE(0x91), USB_DESC_BYTE(0x02),  //   OUTPUT (Data,Var,Abs)
    USB_DESC_BYTE(0x95), USB_DESC_BYTE(0x05),  //   REPORT_COUNT (1)                             = 0x95, 0x01
    USB_DESC_BYTE(0x75), USB_DESC_BYTE(0x01),  //   REPORT_SIZE (3)                              = 0x75, 0x03
    USB_DESC_BYTE(0x91), USB_DESC_BYTE(0x01),  //   OUTPUT (Cnst,Var,Abs)                        = 0x91, 0x03
                                               // --------------------- input report for keycode array
    USB_DESC_BYTE(0x95), USB_DESC_BYTE(0x06),  //   REPORT_COUNT (6)
    USB_DESC_BYTE(0x75), USB_DESC_BYTE(0x08),  //   REPORT_SIZE (8)
    USB_DESC_BYTE(0x15), USB_DESC_BYTE(0x00),  //   LOGICAL_MINIMUM (0)
    USB_DESC_BYTE(0x26), USB_DESC_BYTE(0xff),  //   LOGICAL_MAXIMUM (101)                        = 0x25, 0x65
    USB_DESC_BYTE(0x00),
    USB_DESC_BYTE(0x05), USB_DESC_BYTE(0x07),  //   USAGE_PAGE (Keyboard)
    USB_DESC_BYTE(0x19), USB_DESC_BYTE(0x00),  //   USAGE_MINIMUM (Reserved (no event indicated))
    USB_DESC_BYTE(0x2a), USB_DESC_BYTE(0xff),  //   USAGE_MAXIMUM (Keyboard Application)         = 0x29, 0x65
    USB_DESC_BYTE(0x00),
    USB_DESC_BYTE(0x81), USB_DESC_BYTE(0x00),  //   INPUT (Data,Ary,Abs)
    USB_DESC_BYTE(0xc0)};                      // END_COLLECTION

/*
 * HID Report Descriptor wrapper
 */
static const USBDescriptor hid_report_descriptor = {
    sizeof hid_report_descriptor_data,
    hid_report_descriptor_data};

/*
 * U.S. English language identifier.
 * STRING DESCRIPTOR
 *  bLength: 4
 *  bDescriptorType: 0x03 (STRING)
 *  wLANGID: English (United States) (0x0409)
 */
static const uint8_t hid_string0[] = {
    USB_DESC_BYTE(4),                     /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 *
 * STRING DESCRIPTOR
 *  bLength: 10
 *  bDescriptorType: 0x03 (STRING)
 *  bString: DELL
 */
static uint8_t hid_string1[64];

/*
 * Device Description string.
 *
 * STRING DESCRIPTOR
 *  bLength: 48
 *  bDescriptorType: 0x03 (STRING)
 *  bString: Dell USB Entry Keyboard
 */
static uint8_t hid_string2[64];

/*
 * Strings wrappers array.
 */
static USBDescriptor hid_strings[] = {
    {sizeof hid_string0, hid_string0},
    {sizeof hid_string1, hid_string1},
    {sizeof hid_string2, hid_string2}};

/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                           uint8_t    dtype,
                                           uint8_t    dindex,
                                           uint16_t   lang)
{
    (void)usbp;
    (void)lang;
    switch(dtype) {
        case USB_DESCRIPTOR_DEVICE:
            return &hid_device_descriptor;
        case USB_DESCRIPTOR_CONFIGURATION:
            return &hid_configuration_descriptor;
        case USB_DESCRIPTOR_STRING:
            if(dindex < 3)
                return &hid_strings[dindex];
        case USB_DESCRIPTOR_INTERFACE:
            break;
        case USB_DESCRIPTOR_ENDPOINT:
            break;
        case USB_DESCRIPTOR_HID:
            return &hid_descriptor;
        case HID_REPORT:
            return &hid_report_descriptor;
        default:
            break;
    }
    return NULL;
}

/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState ep1instate;

/**
 * @brief   OUT EP1 state.
 */
static USBOutEndpointState ep1outstate;

/**
 * @brief   EP1 initialization structure (both IN and OUT).
 */
static const USBEndpointConfig ep1config = {
    USB_EP_MODE_TYPE_INTR,
    NULL,
    hidDataTransmitted,
    hidDataReceived,
    0x0040,
    0x0040,
    &ep1instate,
    &ep1outstate,
    2,
    NULL};

/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event)
{
    switch(event) {
        case USB_EVENT_CONFIGURED:
            /* 
             * Enables the endpoints specified into the configuration.
             * Note, this callback is invoked from an ISR so I-Class functions
             * must be used.
             */
            chSysLockFromISR();
            usbInitEndpointI(usbp, USBD2_DATA_REQUEST_EP, &ep1config);
            /* Resetting the state of the CDC subsystem.*/
            hidConfigureHookI(&UHD2);
            chSysUnlockFromISR();
            return;
        case USB_EVENT_ADDRESS:
            return;
        case USB_EVENT_RESET:
            return;
        case USB_EVENT_UNCONFIGURED:
            return;
        case USB_EVENT_SUSPEND:
            return;
        case USB_EVENT_WAKEUP:
            return;
        case USB_EVENT_STALLED:
            return;
    }
    return;
}

uint8_t led_status;
bool    caps_lock, num_lock, scroll_lock;

static bool req_handler(USBDriver *usbp)
{
    if((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS) {
        switch(usbp->setup[1]) {
            case HID_SET_REPORT:
                usbSetupTransfer(usbp, &led_status, 1, NULL);

                //TODO
                caps_lock   = 0;
                num_lock    = 1;
                scroll_lock = 0;

                return true;
            default:
                // Do nothing
                return hidRequestsHook(usbp);
        }
    }
    return hidRequestsHook(usbp);
}

/*
 * USB driver configuration.
 */
const USBConfig usbcfg = {
    usb_event,
    get_descriptor,
    req_handler,
    NULL};

/*
 * USB HID driver configuration.
 */
const USBHIDConfig usbhidcfg = {
    &USBD2,
    USBD2_DATA_REQUEST_EP,
    USBD2_DATA_AVAILABLE_EP};

volatile char         _keyboard_storage_start;
volatile char         _keyboard_storage_end;
static volatile char *passwords = &_keyboard_storage_start;

static void set_device_descriptor(usbh_device_descriptor_t *const device_descriptor);
static void set_string_descriptor(usbh_device_t *const device);

/*
 * USB initialization get on the demo file 
 * ChibiOS/demos/STM32/RT-STM32F407-OLIMEX_E407-LWIP-FATFS-USB/main.c
 */
void usb_init(void)
{
    /*
   * Initializes USB_HS GPIO.
   */
    palSetPadMode(GPIOB, GPIOB_OTG_HS_VBUS, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, GPIOB_OTG_HS_DM, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOB, GPIOB_OTG_HS_DP, PAL_MODE_ALTERNATE(12));
}

void usb_start(USBHDriver *usbh)
{
    /*
    * Update USB descriptors
    */
    usbh_device_t *const usbh_device = &usbh->rootport.device;
    set_device_descriptor(&usbh_device->devDesc);
    set_string_descriptor(usbh_device);

    /*
   * Initializes a serial-over-USB CDC driver.
   */
    hidObjectInit(&UHD2);
    hidStart(&UHD2, &usbhidcfg);

    /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
    usbDisconnectBus(usbhidcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(usbhidcfg.usbp, &usbcfg);
    usbConnectBus(usbhidcfg.usbp);
}

static void set_device_descriptor(usbh_device_descriptor_t *const device_descriptor)
{
    uint8_t usbh_device_descriptor_data[18] = {
        USB_DESC_DEVICE(0x0110, /* bcdUSB (1.1).                    */
                        0x00,   /* bDeviceClass (CDC).              */
                        0x00,   /* bDeviceSubClass.                 */
                        0x00,   /* bDeviceProtocol.                 */
                        0x40,   /* bMaxPacketSize. Fail with 0x08 ! */
                        device_descriptor->idVendor,
                        device_descriptor->idProduct,
                        device_descriptor->bcdDevice,
                        1, /* iManufacturer.                   */
                        2, /* iProduct.                        */
                        0, /* iSerialNumber.                   */
                        1) /* bNumConfigurations.              */
    };

    for(unsigned int i                = 0; i < USB_DESC_DEVICE_SIZE; i++)
        hid_device_descriptor_data[i] = usbh_device_descriptor_data[i];
}

static void set_string_descriptor(usbh_device_t *const device)
{
    usbh_device_descriptor_t *const device_descriptor = &device->devDesc;
    USBH_DEFINE_BUFFER(char str[64]);

    /* Manufacturer */
    usbhDeviceReadString(device, str, sizeof(str), device_descriptor->iManufacturer, device->langID0);

    hid_strings[1].ud_size = USB_DESC_BYTE(strlen(str) * 2 + 2);
    hid_string1[0]         = USB_DESC_BYTE(strlen(str) * 2 + 2);   /* bLength. */
    hid_string1[1]         = USB_DESC_BYTE(USB_DESCRIPTOR_STRING); /* bDescriptorType. */

    for(unsigned int i = 0; i < strlen(str); i++) {
        hid_string1[2 + i * 2]     = str[i];
        hid_string1[2 + i * 2 + 1] = 0;
    }

    /* Product */
    usbhDeviceReadString(device, str, sizeof(str), device_descriptor->iProduct, device->langID0);

    hid_strings[2].ud_size = USB_DESC_BYTE(strlen(str) * 2 + 2);
    hid_string2[0]         = USB_DESC_BYTE(strlen(str) * 2 + 2);   /* bLength. */
    hid_string2[1]         = USB_DESC_BYTE(USB_DESCRIPTOR_STRING); /* bDescriptorType. */

    for(unsigned int i = 0; i < strlen(str); i++) {
        hid_string2[2 + i * 2]     = str[i];
        hid_string2[2 + i * 2 + 1] = 0;
    }
}

void usb_report(USBHIDDriver *uhdp, uint8_t *bp, uint8_t n)
{
    if(usbhidcfg.usbp->state == USB_ACTIVE) {
        hidWriteReport(uhdp, bp, n);
    }
}

static void usb_send_key(USBHIDDriver *uhdp, uint8_t modifier, uint8_t key)
{
    uint8_t report_key[8] = {modifier, 0, key, 0, 0, 0, 0, 0};
    usb_report(uhdp, report_key, 8);
}

void usb_password_terminal(USBHIDDriver *uhdp)
{
    int     index    = 0;
    uint8_t last_key = 0x00;
    //all input from passwords
    while(passwords[index] != 0x00FF) {
        uint8_t key      = (uint8_t)passwords[index];
        uint8_t modifier = passwords[index] >> 8;
        //only shift and alt Gr is enable
        if(modifier == KEY_MOD_LSHIFT ||
           modifier == KEY_MOD_RSHIFT ||
           modifier == KEY_MOD_RALT ||
           modifier == 0) {
            //against multi same key issue
            if(key == last_key) {
                usb_send_key(uhdp, 0, 0);
                chThdSleepMilliseconds(10);
            }
            last_key = key;
            usb_send_key(uhdp, modifier, key);
            //need to wait
            chThdSleepMilliseconds(10);
        }
        index++;
    }

    //unpress input
    usb_send_key(uhdp, 0, 0);
}
