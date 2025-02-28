#ifndef USBSTANDARD_H_
#define USBSTANDARD_H_
#include <stdint.h>

// Max packet size for USB FS
#define MAX_PACKET_SIZE_BYTES_FS		64U
#define USB_CONFIG_BUS_POWERED			0x80
#define USB_CONFIG_SELF_POWERED			0x40
#define USB_SUBCLASS_NONE				0x00
#define USB_PROTOCOL_NONE				0x00



#define USB_STRING_DESCRIPTOR_TYPE		0x03
#define USB_STRING_LANGID_ENGLISH		0x0409

#define USB_STRING_DESCRIPTOR_INDEX_0				0x00
#define USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER 	0x01
#define USB_STRING_DESCRIPTOR_INDEX_PRODUCT			0x02
#define USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUM		0x03

#define USB_BM_REQUEST_TYPE_DIRECTION_MASK 		(1U << 7)
#define USB_BM_REQUEST_TYPE_DIRECTION_TODEVICE 	(0U << 7)
#define USB_BM_REQUEST_TYPE_DIRECTION_TOHOST 	(1U << 7)

#define USB_BM_REQUEST_TYPE_TYPE_MASK 			(3U << 5)
#define USB_BM_REQUEST_TYPE_TYPE_STANDARD 		(0U << 5)
#define USB_BM_REQUEST_TYPE_TYPE_CLASS 			(1U << 5)
#define USB_BM_REQUEST_TYPE_TYPE_VENDOR 		(2U << 5)

#define USB_BM_REQUEST_TYPE_RECIPIENT_MASK 		(3U << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE 	(0U << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE (1U << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_ENDPOINT 	(2U << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_OTHER 	(3U << 0)

#define USB_EP_DIR_MSK							(0x80)
#define USB_EP_NUM_MSK							(0x7F)
#define USB_EP_TYPE_MSK							(0x3)

#define USB_WEB_GET_URL				(0xC0)


#define USB_CLASS_APP_SPEC 			0xFE
#define USB_CLASS_VENDOR 			0xFF
#define USB_CLASS_AUDIO				0x01
#define USB_CLASS_HID 				0x03
#define USB_SUBCLASS_VENDOR 		0xFF

#define USB_CDC_COMM_CLASS			0x02

#define USB_CLASS_PER_INTERFACE 	0x00
#define USB_CLASS_APP_SPEC 			0xFE
#define USB_CLASS_VENDOR 			0xFF
#define USB_CLASS_CDC				0x02
#define USB_CLASS_HID 				0x03

#define USB_SUBCLASS_NONE 			0x00
#define USB_SUBCLASS_AUDIO_CONTROL	0x01
#define USB_SUBCLASS_IAD 			0x02
#define USB_SUBCLASS_VENDOR 		0xFF

#define USB_PROTOCOL_NONE 			0x00
#define USB_PROTOCOL_IAD 			0x01
#define USB_PROTOCOL_VENDOR 		0xFF

#define USB_CS_INTERFACE			0x24
#define USB_CS_ENDPOINT				0x25
#define USB_CS_HEADER				0x01

#define USB_WEB_GUID				{0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47, 0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65}

typedef enum {
	USB_ENDPOINT_TYPE_CONTROL = 0,
	USB_ENDPOINT_TYPE_ISOCHRONOUS,
	USB_ENDPOINT_TYPE_BULK,
	USB_ENDPOINT_TYPE_INTERRUPT,
} USB_ENDPOINT_TYPE;

typedef enum {
	USB_ENDPOINT_DIR_OUT = 0x00,
	USB_ENDPOINT_DIR_IN,
} USB_ENDPOINT_DIR;

typedef struct {
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} UsbRequest;

typedef enum {
	USB_STANDARD_DEVICE_REQUEST_GET_STATUS = 0,
	USB_STANDARD_DEVICE_REQUEST_CLEAR_FEATURE,
	USB_STANDARD_DEVICE_REQUEST_RESERVED_0x2,
	USB_STANDARD_DEVICE_REQUEST_SET_FEATURE,
	USB_STANDARD_DEVICE_REQUEST_RESERVED_0x4,
	USB_STANDARD_DEVICE_REQUEST_SET_ADDRESS,
	USB_STANDARD_DEVICE_REQUEST_GET_DESCRIPTOR,
	USB_STANDARD_DEVICE_REQUEST_SET_DESCRIPTOR ,
	USB_STANDARD_DEVICE_REQUEST_GET_CONFIGURATION,
	USB_STANDARD_DEVICE_REQUEST_SET_CONFIGURATION,
	USB_STANDARD_DEVICE_REQUEST_GET_INTERFACE,
	USB_STANDARD_DEVICE_REQUEST_SET_INTERFACE,

} USB_STANDARD_DEVICE_REQUESTS;

typedef enum {
	USB_DESCRIPTOR_TYPE_DEVICE = 1,
	USB_DESCRIPTOR_TYPE_CONFIGURATION,
	USB_DESCRIPTOR_TYPE_STRING,
	USB_DESCRIPTOR_TYPE_INTERFACE,
	USB_DESCRIPTOR_TYPE_ENDPOINT,
	USB_DESCRIPTOR_TYPE_QUALIFIER,
	USB_DESCRIPTOR_TYPE_OTHER,
	USB_DESCRIPTOR_TYPE_INTERFACEPOWER,
	USB_DESCRIPTOR_TYPE_OTG,
	USB_DESCRIPTOR_TYPE_DEBUG,
	USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
	
	USB_DESCRIPTOR_TYPE_BOS = 0x0F,
	USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY,

} USB_DESCRIPTOR_TYPE;

typedef enum {
	USB_VENDOR_SPECIFIC_DEVICE_REQUEST_MS_OS_DESCRIPTOR = 0xEE,
} USB_VENDOR_SPECIFIC_DEVICE_REQUEST;

typedef enum {
	USB_VENDOR_SPECIFIC_DESCRIPTOR_MS_OS1 = 0x0007
} USB_VENDOR_SPECIFIC_DESCRIPTOR;

typedef enum {
	USB_DEVICE_STATE_DEFAULT,
	USB_DEVICE_STATE_ADDRESSED,
	USB_DEVICE_STATE_CONFIGURED,
	USB_DEVICE_STATE_SUSPENDED,
} USB_DEVICE_STATE;

typedef enum {
	USB_WEB_SCHEME_PREFIX_HTTP = 0,
	USB_WEB_SCHEME_PREFIX_HTTPS,
	USB_WEB_SCHEME_NO_PREFIX = 255
} USB_WEB_SCHEME_PREFIX;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
} __attribute__((__packed__)) UsbDeviceStandardDescriptor;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wTotalLength;
	uint8_t  bNumInterfaces;
	uint8_t  bConfigurationValue;
	uint8_t  iConfiguration;
	uint8_t  bmAttributes;
	uint8_t  bMaxPower;
} __attribute__((__packed__)) UsbConfigurationDescriptor;


typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} __attribute__((__packed__)) UsbInterfaceDescriptor;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bEndpointAddress;
	uint8_t  bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t  bInterval;
} __attribute__((__packed__)) UsbEndpointDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wLangID0;
} __attribute__((__packed__)) UsbStringDescriptor0;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t *bString;
} __attribute__((__packed__)) UsbStringDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bFirstInterface;
	uint8_t bInterfaceCount;
	uint8_t bFunctionClass;
	uint8_t bFunctionSubClass;
	uint8_t bFunctionProtocol;
	uint8_t iFunction;
} UsbInterfaceAssociationDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumDeviceCaps;
} __attribute__((__packed__)) UsbBinaryDeviceObjectStoreDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint8_t bReserved;
	uint8_t platformCapabilityUUID[16];
	uint16_t bcdVersion;
	uint8_t bVendorCode;
	uint8_t iLandingPage;
} __attribute__((__packed__)) UsbWebPlatformCapabilityDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bScheme;
	char url[30];
} __attribute__((__packed__)) UsbWebURLDescriptor;

#endif