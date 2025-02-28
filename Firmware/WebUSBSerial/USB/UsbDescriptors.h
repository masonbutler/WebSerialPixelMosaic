#ifndef USBDESCRIPTORS_H_
#define USBDESCRIPTORS_H_
#include "Standard/UsbStandard.h"
#include "Standard/UsbCdcStandard.h"
#include "WindowsSpecific/WinUsb.h"

#define VENDOR_ID	(0xFFFE)
#define PRODUCT_ID	(0xEEEA)

#define USB_EP_CDC_RECEIVE	0x2
#define USB_EP_CDC_SEND		0x81
#define USB_EP_CDC_NOTIFY	0x83

#define STR_DESC_SIZE(LEN) (2U + (2U * (LEN)))

#define USB_STR_MANUFACTURER	'M', 0x00U, 'a', 0x00U, 's', 0x00U, 'o', 0x00U, 'n', 0x00U, ' ', 0x00U, 'B', 0x00U, 'u', 0x00U, 't', 0x00U, 'l', 0x00U, 'e', 0x00U, 'r', 0x00U
#define USB_STR_MANUFACTURER_LEN	12U

#define USB_STR_PRODUCT 'L', 0x00U, 'E', 0x00U, 'D', 0x00U, ' ', 0x00U, 'M', 0x00U, 'O', 0x00U, 'S', 0x00U, 'A', 0x00U, 'I', 0x00U, 'C', 0x00U
#define USB_STR_PRODUCT_LEN	10U

#define USB_STR_SERIAL_NUM	'4', 0x00U, '4', 0x00U, '7',0x00U, '3', 0x00U, '6', 0x00U, '3', 0x00U, '7', 0x00U, '5', 0x00U, '9', 0x00U
#define USB_STR_SERIAL_NUM_LEN	9U

typedef struct {
	UsbConfigurationDescriptor usbConfigurationDescriptor;
	UsbInterfaceAssociationDescriptor usbInterfaceAssociationDescriptor;
	UsbInterfaceDescriptor usbControlInterfaceDescriptor;
	
	UsbCdcHeaderFunctionalDescriptor usbCdcFuncHeaderDescriptor;
	UsbCdcUnionFunctionalDescriptor usbCdcUnionFuncDescriptor;
	
	UsbEndpointDescriptor usbEpNotificationDescriptor;
	UsbInterfaceDescriptor usbCdcDataInterfaceDescriptor;
	
	UsbEndpointDescriptor usbEpCommInDescriptor;
	UsbEndpointDescriptor usbEpCommOutDescriptor;
} __attribute__((__packed__))  UsbCombinedDescriptor;

typedef struct {
	UsbBinaryDeviceObjectStoreDescriptor usbBosDescriptor;
	UsbWebPlatformCapabilityDescriptor usbWebCapabilityDescriptor;
	UsbMSOS20PlatformCapabilityDescriptor usbMsCapabilityDescriptor;
	
} __attribute__((__packed__)) UsbBOSCombinedDescriptor;

typedef struct {
	UsbMSOS20SetHeaderDescriptor usbMsSetHeaderDescriptor;
	UsbMSOS20ConfigurationSubsetDescriptor usbMsConfigSubsetDescriptor;
	UsbMSOS20FunctionSubsetDescriptor usbMsFunctionSubsetDescriptor;
	UsbMSOS20CompatibleIdDescriptor usbMsCompatibleIdDescriptor;
	UsbMSOS20RegistryPropertyDescriptor usbMsRegPropertyDescriptor;
	
} __attribute__((__packed__)) UsbMSOS20Descriptors;

extern UsbDeviceStandardDescriptor usbDeviceDescriptor;
extern UsbCombinedDescriptor usbCombinedDescriptor;
extern UsbBOSCombinedDescriptor usbBosCombinedDescriptor;
extern UsbMSOS20Descriptors usbMsDescriptor;

extern uint8_t usbStringDescriptor0[4];
extern uint8_t usbStringDescManufacturer[STR_DESC_SIZE(USB_STR_MANUFACTURER_LEN)];
extern uint8_t usbStringDescProduct[STR_DESC_SIZE(USB_STR_PRODUCT_LEN)];
extern uint8_t usbStringDescSerialNumber[STR_DESC_SIZE(USB_STR_SERIAL_NUM_LEN)];


#endif