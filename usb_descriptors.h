#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include "tusb.h"

// Device descriptor
extern tusb_desc_device_t const desc_device;

// Configuration descriptor
extern uint8_t const desc_configuration[];

// Callback functions
extern uint8_t const* tud_descriptor_device_cb(void);
extern uint8_t const* tud_descriptor_configuration_cb(uint8_t index);
extern uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid);
extern uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
extern void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);
extern bool hidd_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request);

#endif // DESCRIPTORS_H
