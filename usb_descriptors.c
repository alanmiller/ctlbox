#include "usb_descriptors.h"
#include "hardware/regs/addressmap.h"
#include "pico/unique_id.h"

uint16_t string_desc[32];
uint32_t serial_number[4];

pico_unique_board_id_t board_id;
char serial_number_str[25];

void get_serial_number() {
    pico_get_unique_board_id(&board_id);
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; ++i) {
        snprintf(serial_number_str + i * 2, 3, "%02x", board_id.id[i]);
    }
}

char const *string_desc_arr [] = {
    (const char[]) {0x09, 0x04}, // 0: is supported language is English (0x0409)
    "AlanMiller",                // 1: Manufacturer
    "GolfSimController",         // 2: Product
    serial_number_str,           // 3: Serials, should use chip ID
};

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

    .iManufacturer      = 1, // Index of Manufacturer string descriptor
    .iProduct           = 2, // Index of Product string descriptor
    .iSerialNumber      = 3, // Index of Serial number string descriptor

    .bNumConfigurations = 0x01
};
// URL for the firmware
char const *url = "https://github.com/alanmiller/ctlbox";

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
    0x01, // bInterfaceProtocol keyboard 1, mouse 2
    0x00, // iInterface (String Index)

    // HID descriptor
    0x09, // bLength
    0x21, // bDescriptorType (HID)
    0x11, 0x01, // bcdHID 1.11
    0x00, // bCountryCode
    0x01, // bNumDescriptors
    0x22, // bDescriptorType[0] (HID)
    0x43, 0x00, // wDescriptorLength[0] 67

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

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    return desc_hid_report;
}

//uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance) {
//    return desc_hid_report;
//}

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
    
    printf("tud_descriptor_string_cb called with index %d, langid %04x\n", index, langid);
    printf("HID report descriptor length: %zu\n", sizeof(desc_hid_report));

    (void) langid;
    uint8_t chr_count = 0;

    if (index == 0) {
        memcpy(string_desc, string_desc_arr[0], 2);
        chr_count = 1;
        printf("Language ID: 0x%04x\n", string_desc[1]);
    } else {
        // Convert ASCII string into UTF-16
        if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0]))) return NULL;
        const char* str = string_desc_arr[index];

        // Cap at max char
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;

        string_desc[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

        for(uint8_t i=0; i<chr_count; i++) {
            string_desc[1+i] = str[i];
            }
    }
    // Print the string descriptor
    printf("String descriptor: ");
    for (uint8_t i = 0; i < chr_count; i++) {
        printf("%c", string_desc[1+i]);
    }
    printf("\n");
    return string_desc;
}

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    return (uint8_t const *)&desc_configuration;
}