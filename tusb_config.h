#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#define CFG_TUSB_RHPORT0_MODE       (OPT_MODE_DEVICE | OPT_MODE_HIGH_SPEED)

// Device mode with CDC and HID
#define CFG_TUD_CDC                 1
#define CFG_TUD_HID                 1

#define CFG_TUD_CDC_RX_BUFSIZE      64
#define CFG_TUD_CDC_TX_BUFSIZE      64

#define CFG_TUD_HID_EP_BUFSIZE      16

#endif // _TUSB_CONFIG_H_