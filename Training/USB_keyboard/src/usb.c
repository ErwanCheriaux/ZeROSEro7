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

#include "usb.h"
#include "rtt.h"

uint8_t increment_var = 0;

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
static const uint8_t hid_device_descriptor_data[18] = {
    USB_DESC_DEVICE(0x0110, /* bcdUSB (1.1).                    */
                    0x00,   /* bDeviceClass (CDC).              */
                    0x00,   /* bDeviceSubClass.                 */
                    0x00,   /* bDeviceProtocol.                 */
                    0x40,   /* bMaxPacketSize. Fail with 0x08 ! */
                    0x413c, /* idVendor (ST).                   */
                    0x2107, /* idProduct.                       */
                    0x0178, /* bcdDevice.                       */
                    1,      /* iManufacturer.                   */
                    2,      /* iProduct.                        */
                    0,      /* iSerialNumber.                   */
                    1)      /* bNumConfigurations.              */
};

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
                 34),    /* wDescriptorLength                    */
                         /* 65 :Have to update report_descriptor */
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
    USB_DESC_BYTE(0x06),   /* Usage Page -                     */
    USB_DESC_WORD(0xFF00), /*   Vendor Defined.                */
    USB_DESC_BYTE(0x09),   /* Usage -                          */
    USB_DESC_BYTE(0x01),   /*   Vendor Defined.                */
    USB_DESC_BYTE(0xA1),   /* Collection -                     */
    USB_DESC_BYTE(0x01),   /*   Application.                   */

    USB_DESC_BYTE(0x09),   /* Usage -                          */
    USB_DESC_BYTE(0x01),   /*   Vendor Defined.                */
    USB_DESC_BYTE(0x15),   /* Logical Minimum -                */
    USB_DESC_BYTE(0x00),   /*   0.                             */
    USB_DESC_BYTE(0x26),   /* Logical Maximum -                */
    USB_DESC_WORD(0x00FF), /*   255.                           */
    USB_DESC_BYTE(0x75),   /* Report size -                    */
    USB_DESC_BYTE(0x08),   /*   8 bits.                        */
    USB_DESC_BYTE(0x95),   /* Report count -                   */
    USB_DESC_BYTE(0x01),   /*   1.                             */
    USB_DESC_BYTE(0x81),   /* Input -                          */
    USB_DESC_BYTE(0x02),   /*   Data, Variable, Absolute.      */

    USB_DESC_BYTE(0x09),   /* Usage -                          */
    USB_DESC_BYTE(0x01),   /*   Vendor Defined.                */
    USB_DESC_BYTE(0x15),   /* Logical Minimum -                */
    USB_DESC_BYTE(0x00),   /*   0.                             */
    USB_DESC_BYTE(0x26),   /* Logical Maximum -                */
    USB_DESC_WORD(0x00FF), /*   255.                           */
    USB_DESC_BYTE(0x75),   /* Report Size -                    */
    USB_DESC_BYTE(0x08),   /*   8 bits.                        */
    USB_DESC_BYTE(0x95),   /* Report Count -                   */
    USB_DESC_BYTE(0x01),   /*   1.                             */
    USB_DESC_BYTE(0x91),   /* Output -                         */
    USB_DESC_BYTE(0x02),   /*   Data, Variable, Absolute.      */

    USB_DESC_BYTE(0xC0) /* End Collection.                  */
};

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
static const uint8_t hid_string1[] = {
    USB_DESC_BYTE(10),                    /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    'D', 0, 'E', 0, 'L', 0, 'L', 0};

/*
 * Device Description string.
 *
 * STRING DESCRIPTOR
 *  bLength: 48
 *  bDescriptorType: 0x03 (STRING)
 *  bString: Dell USB Entry Keyboard
 */
static const uint8_t hid_string2[] = {
    USB_DESC_BYTE(48),                    /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    'D', 0, 'e', 0, 'l', 0, 'l', 0, ' ', 0, 'U', 0, 'S', 0, 'B', 0,
    ' ', 0, 'E', 0, 'n', 0, 't', 0, 'r', 0, 'y', 0, ' ', 0, 'K', 0,
    'e', 0, 'y', 0, 'b', 0, 'o', 0, 'a', 0, 'r', 0, 'd', 0};

/*
 * Strings wrappers array.
 */
static const USBDescriptor hid_strings[] = {
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
    USB_EP_MODE_TYPE_BULK,
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
            chSysLockFromISR();

            /* Enables the endpoints specified into the configuration.
       Note, this callback is invoked from an ISR so I-Class functions
       must be used.*/
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

static bool req_handler(USBDriver *usbp)
{
    size_t n;

    if((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS) {
        switch(usbp->setup[1]) {
            case HID_GET_REPORT:
                n = hidGetReport(0, &increment_var, sizeof(increment_var));
                usbSetupTransfer(usbp, &increment_var, n, NULL);
                return true;
            default:
                return hidRequestsHook(usbp);
        }
    }
    return hidRequestsHook(usbp);
}

/**
 * @brief   Generate HID Report
 * @details This function generates the data for an HID report so
 *          that it can be transferred to the host.
 *
 * @param[in]  id       report ID
 * @param[out] bp       data buffer pointer
 * @param[in]  n        the maximum number of bytes for data buffer
 * @return              number of bytes of report in data buffer
 */
size_t hidGetReport(uint8_t id, uint8_t *bp, size_t n)
{
    (void)id;
    (void)n;

    increment_var++;
    *bp = increment_var;
    return sizeof(increment_var);
}

/**
 * @brief   Set HID Report
 * @details This function sets the data for an HID report
 *          that was transferred from the host.
 *
 * @param[in]  id       report ID
 * @param[in]  bp       data buffer pointer
 * @param[in]  n        the number of bytes in data buffer
 * @return              The operation status.
 * @retval MSG_OK       if the report was set.
 */
msg_t hidSetReport(uint8_t id, uint8_t *bp, size_t n)
{
    (void)id;
    (void)n;

    increment_var = *bp;
    return MSG_OK;
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
    palSetPadMode(GPIOB, GPIOB_OTG_HS_ID, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOB, GPIOB_OTG_HS_DM, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOB, GPIOB_OTG_HS_DP, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOA, GPIOA_USB_HS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOF, GPIOF_USB_HS_FAULT, PAL_MODE_INPUT);

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

/*
 * Initialisation of OTG FS port connected to a keyboard.
 */
void usbh_init(void)
{
    /*USBH_FS OTG*/
    palSetPadMode(GPIOA, GPIOA_OTG_FS_VBUS, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, GPIOA_OTG_FS_ID, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DM, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DP, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOB, GPIOB_USB_FS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, GPIOB_USB_FS_FAULT, PAL_MODE_INPUT);

    rtt_printf("Turn on USB power");
    palSetPad(GPIOB, GPIOB_USB_FS_BUSON);
    chThdSleepMilliseconds(100);

    usbhStart(&USBHD1);
}
