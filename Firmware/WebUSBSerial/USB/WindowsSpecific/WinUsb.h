#ifndef WINUSB_H_
#define WINUSB_H_
#include "../../Utility.h"

#define USB_MS_OS20_GUID		{0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C, 0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F}

#define USB_MS_OS20_WINDOWS_VER_8_1		0x06030000
#define USB_MS_OS20_WINDOWS_VER_10		0x06040000

#define MS_OS20_COMPATIBLE_ID_WINUSB	{'W',  'I',  'N',  'U',  'S',  'B',  0x00, 0x00}
	
#define MS_OS20_VENDOR_REQUEST_DESCRIPTOR	0x02
	
typedef enum {
	MS_OS20_DESCRIPTOR_TYPE_SET_HEADER = 0,
	MS_OS20_DESCRIPTOR_TYPE_SUBSET_HEADER_CONFIG,
	MS_OS20_DESCRIPTOR_TYPE_SUBSET_HEADER_FUNC,
	MS_OS20_DESCRIPTOR_TYPE_FEATURE_COMPATIBLE_ID,
	MS_OS20_DESCRIPTOR_TYPE_FEATURE_REG_PROPERTY,
} MS_OS20_DESCRIPTOR_TYPE;


typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint8_t bReserved;
	uint8_t platformCapabilityUUID[16];
	uint32_t dwWindowsVersion;
	uint16_t wDescriptorSetTotalLength;
	uint8_t bVendorCode;
	uint8_t bAltEnumCode;
} __attribute__((__packed__)) UsbMSOS20PlatformCapabilityDescriptor;

typedef struct {
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint32_t dwWindowsVersion;
	uint16_t wTotalLength;
} __attribute__((__packed__)) UsbMSOS20SetHeaderDescriptor;

typedef struct {
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t bConfigurationValue;
	uint8_t bReserved;
	uint16_t wTotalLength;
} __attribute__((__packed__)) UsbMSOS20ConfigurationSubsetDescriptor;

typedef struct {
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t bFirstInterface;
	uint8_t bReserved;
	uint16_t wSubsetLength;
} __attribute__((__packed__)) UsbMSOS20FunctionSubsetDescriptor;

typedef struct {
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t compatibleId[8];
	uint8_t subCompatibleId[8];
} __attribute__((__packed__)) UsbMSOS20CompatibleIdDescriptor;

typedef struct {
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint16_t wPropertyDataType;
	uint16_t wPropertyNameLength;
	uint8_t propertyName[42];
	uint16_t wPropertyDataLength;
	uint8_t propertyData[80];
} __attribute__((__packed__)) UsbMSOS20RegistryPropertyDescriptor;


#endif