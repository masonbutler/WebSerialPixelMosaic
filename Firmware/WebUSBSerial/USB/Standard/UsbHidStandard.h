#ifndef USBHIDSTANDARD_H_
#define USBHIDSTANDARD_H_

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdHID;
	uint8_t bCountryCode;
	uint8_t bNumDescriptors;
	uint8_t bDescriptorType0;
	uint16_t wDescriptorLength0;
} __attribute__((__packed__)) UsbHidDescriptor;

#define USB_HID_COUNTRY_NONE        0x00

#define USB_DESCRIPTOR_TYPE_HID               0x21
#define USB_DESCRIPTOR_TYPE_HID_REPORT        0x22

typedef enum {
	USB_HID_REPORT_IN = 0x00,
	USB_HID_REPORT_OUT,
	USB_HID_REPORT_FEATURE
} USB_HID_REPORT;


typedef enum {
	USB_HID_REQUEST_GET_REPORT = 0x01,
	USB_HID_REQUEST_GET_IDLE,
	USB_HID_REQUEST_GET_PROTOCOL,
	USB_HID_REQUEST_SET_REPORT = 0x09,
	USB_HID_REQUEST_SET_IDLE = 0x0A,
	USB_HID_REQUEST_SET_PROTOCOL = 0x0B,
} USB_HID_REQUEST;

  




#endif