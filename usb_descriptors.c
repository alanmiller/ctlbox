#include "usb_descriptors.h"

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0x1234, // Vendor ID
    .idProduct          = 0x5678, // Product ID
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01, // Index of Manufacturer string descriptor
    .iProduct           = 0x02, // Index of Product string descriptor
    .iSerialNumber      = 0x03, // Index of Serial number string descriptor

    .bNumConfigurations = 0x01
};
uint8_t const desc_configuration[] = {
    // Configuration descriptor
    0x09, // bLength
    0x02, // bDescriptorType (Configuration)
    0x22, 0x00, // wTotalLength 34
    0x01, // bNumInterfaces 1
    0x01, // bConfigurationValue
    0x00, // iConfiguration (String Index)
    0x80, // bmAttributes Self Powered
    0x32, // bMaxPower 100mA

    // Interface descriptor
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x00, // bInterfaceNumber 0
    0x00, // bAlternateSetting
    0x01, // bNumEndpoints 1
    0x03, // bInterfaceClass
    0x01, // bInterfaceSubClass
    0x02, // bInterfaceProtocol
    0x00, // iInterface (String Index)

    // HID descriptor
    0x09, // bLength
    0x21, // bDescriptorType (HID)
    0x11, 0x01, // bcdHID 1.11
    0x00, // bCountryCode
    0x01, // bNumDescriptors
    0x22, // bDescriptorType[0] (HID)
    0x34, 0x00, // wDescriptorLength[0] 52

    // Endpoint descriptor
    0x07, // bLength
    0x05, // bDescriptorType (Endpoint)
    0x81, // bEndpointAddress (IN/D2H)
    0x03, // bmAttributes (Interrupt)
    0x08, 0x00, // wMaxPacketSize 8
    0x0A // bInterval 10 (unit depends on device speed)
};

uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1))
};

// Callback functions
uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance) {
    return desc_hid_report;
}

uint8_t const* tud_descriptor_device_cb(void) {
    return (uint8_t const*)&desc_device;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    // Do nothing for now. Here we would respond to requests to return the state of leds, modifiers, etc.
    return 0;
}
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    // Do nothing for now. Later we would control an led or something here.
}
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    return NULL; // No string descriptors
}
uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    return (uint8_t const *)&desc_configuration;
}
//bool hidd_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request) {
//    return false; // Request not handled
//}