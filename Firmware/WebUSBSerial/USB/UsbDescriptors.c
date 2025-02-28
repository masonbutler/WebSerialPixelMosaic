#include "UsbDescriptors.h"

UsbDeviceStandardDescriptor usbDeviceDescriptor = {
	.bLength = sizeof(UsbDeviceStandardDescriptor),
	.bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE,
	.bcdUSB = 0x0210,
	.bDeviceClass = 0x02, //USB_CLASS_CDC,
	.bDeviceSubClass = 0x02,// USB_SUBCLASS_NONE
	.bDeviceProtocol = 0x1, // USB_DEVICE_PROTOCOL_INTERFACE_ASSOCIATION
 	.bMaxPacketSize0 = 64,
	.idVendor = VENDOR_ID,
	.idProduct = PRODUCT_ID,
	.bcdDevice = 0x0100,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

UsbCombinedDescriptor usbCombinedDescriptor = {
	.usbConfigurationDescriptor = {		
		.bLength = sizeof(UsbConfigurationDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_CONFIGURATION,
		.wTotalLength = sizeof(UsbCombinedDescriptor),
		.bNumInterfaces = 2,
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_BUS_POWERED | USB_CONFIG_SELF_POWERED,
		.bMaxPower = 50,
	},
	
	.usbInterfaceAssociationDescriptor = {
		.bLength = sizeof(UsbInterfaceAssociationDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
		.bFirstInterface = 0,
		.bInterfaceCount = 2,
		.bFunctionClass = USB_CDC_COMM_CLASS,
		.bFunctionSubClass = USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,
		.bFunctionProtocol = 0,
		.iFunction = 0,
	},
	
	.usbControlInterfaceDescriptor = {
		.bLength = sizeof(UsbInterfaceDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 1,
		.bInterfaceClass = USB_CDC_COMM_CLASS,
		.bInterfaceSubClass = USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,
		.bInterfaceProtocol = 0,
		.iInterface = 0,
	},
	
	.usbCdcFuncHeaderDescriptor =
	{
		.bFunctionalLength = sizeof(UsbCdcHeaderFunctionalDescriptor),
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubType = USB_CDC_SUBTYPE_HEADER,
		.bcdCDC = 0x0110,		
	},
	
	.usbCdcUnionFuncDescriptor = {
		.bFunctionalLength = sizeof(UsbCdcUnionFunctionalDescriptor),
		.bDescriptorType = USB_CS_INTERFACE,
		.bDescriptorSubType = USB_CDC_SUBTYPE_UNION,
		.bControlInterface = 0,
		.bSubordinateInferface0 = 1,
	},
	
	.usbEpNotificationDescriptor = {
		.bLength = sizeof(UsbEndpointDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = USB_EP_CDC_NOTIFY,
		.bmAttributes = USB_ENDPOINT_TYPE_INTERRUPT,
		.wMaxPacketSize = 64,
		.bInterval = 1,
	},
	
	.usbCdcDataInterfaceDescriptor = {
		.bLength = sizeof(UsbInterfaceDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 1,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = USB_CDC_DATA_INTERFACE_CLASS,
		.bInterfaceSubClass = USB_SUBCLASS_NONE,
		.bInterfaceProtocol = USB_PROTOCOL_NONE,
		.iInterface = 0,
	},
	
	.usbEpCommInDescriptor = {
		.bLength = sizeof(UsbEndpointDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = USB_EP_CDC_SEND,
		.bmAttributes = USB_ENDPOINT_TYPE_BULK,
		.wMaxPacketSize = 64,
		.bInterval = 0,
	},
	
	.usbEpCommOutDescriptor = {
		.bLength = sizeof(UsbEndpointDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = USB_EP_CDC_RECEIVE, 
		.bmAttributes = USB_ENDPOINT_TYPE_BULK,
		.wMaxPacketSize = 64,
		.bInterval = 0,
	},
};

UsbBOSCombinedDescriptor usbBosCombinedDescriptor = {
	.usbBosDescriptor = {
		.bLength = sizeof(UsbBinaryDeviceObjectStoreDescriptor),
		.bDescriptorType = USB_DESCRIPTOR_TYPE_BOS,
		.wTotalLength = sizeof(UsbBOSCombinedDescriptor),
		.bNumDeviceCaps = 2,
	},
	.usbWebCapabilityDescriptor = {
		.bLength = sizeof(UsbWebPlatformCapabilityDescriptor),
		.bDescriptorType = 0x10,
		.bDevCapabilityType = 0x5,
		.bReserved = 0,
		.platformCapabilityUUID = USB_WEB_GUID,
		.bcdVersion = 0x0100,
		.bVendorCode = 1,
		.iLandingPage = 0,
	},
	.usbMsCapabilityDescriptor = { 
		.bLength = sizeof(UsbMSOS20PlatformCapabilityDescriptor),
		.bDescriptorType = 0x10,
		.bDevCapabilityType = 0x05,
		.bReserved = 0,
		.platformCapabilityUUID = USB_MS_OS20_GUID,
		.dwWindowsVersion = USB_MS_OS20_WINDOWS_VER_8_1,
		.wDescriptorSetTotalLength = 0xB2,
		.bVendorCode = MS_OS20_VENDOR_REQUEST_DESCRIPTOR,
		.bAltEnumCode = 0x00,
	},
	
};
	
UsbMSOS20Descriptors usbMsDescriptor = {
	.usbMsSetHeaderDescriptor = {					
		.wLength = sizeof(UsbMSOS20SetHeaderDescriptor),
		.wDescriptorType = MS_OS20_DESCRIPTOR_TYPE_SET_HEADER,
		.dwWindowsVersion = USB_MS_OS20_WINDOWS_VER_8_1,
		.wTotalLength = sizeof(UsbMSOS20Descriptors),
	},
	.usbMsConfigSubsetDescriptor = {			
		.wLength = sizeof(UsbMSOS20ConfigurationSubsetDescriptor),
		.wDescriptorType = MS_OS20_DESCRIPTOR_TYPE_SUBSET_HEADER_CONFIG,
		.bConfigurationValue = 0,
		.bReserved = 0,
		.wTotalLength = sizeof(UsbMSOS20Descriptors) - sizeof(UsbMSOS20SetHeaderDescriptor),
	},
	.usbMsFunctionSubsetDescriptor = {		
		.wLength = sizeof(UsbMSOS20FunctionSubsetDescriptor),
		.wDescriptorType = MS_OS20_DESCRIPTOR_TYPE_SUBSET_HEADER_FUNC,
		.bFirstInterface = 1,
		.bReserved = 0,
		.wSubsetLength = sizeof(UsbMSOS20Descriptors) - (sizeof(UsbMSOS20SetHeaderDescriptor) + sizeof(UsbMSOS20FunctionSubsetDescriptor)),
	},
	.usbMsCompatibleIdDescriptor = {			
		.wLength = sizeof(UsbMSOS20CompatibleIdDescriptor),
		.wDescriptorType = MS_OS20_DESCRIPTOR_TYPE_FEATURE_COMPATIBLE_ID,
		.compatibleId = MS_OS20_COMPATIBLE_ID_WINUSB,
		.subCompatibleId = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
	},
	.usbMsRegPropertyDescriptor = {  
		 .wLength= sizeof(UsbMSOS20RegistryPropertyDescriptor),
		 .wDescriptorType = MS_OS20_DESCRIPTOR_TYPE_FEATURE_REG_PROPERTY,
		 .wPropertyDataType = 0x0007,
		 .wPropertyNameLength = 0x2A,
		 .propertyName = {'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
							'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00,
							 0x00, 0x00},
		 .wPropertyDataLength = 0x50,
		 .propertyData = {'{', 0x00, '9', 0x00, '7', 0x00, '5', 0x00, 'F', 0x00, '4', 0x00, '4', 0x00, 'D', 0x00, '9', 0x00, '-', 0x00,
			 '0', 0x00, 'D', 0x00, '0', 0x00, '8', 0x00, '-', 0x00, '4', 0x00, '3', 0x00, 'F', 0x00, 'D', 0x00, '-', 0x00,
			 '8', 0x00, 'B', 0x00, '3', 0x00, 'E', 0x00, '-', 0x00, '1', 0x00, '2', 0x00, '7', 0x00, 'C', 0x00, 'A', 0x00,
		 '8', 0x00, 'A', 0x00, 'F', 0x00, 'F', 0x00, 'F', 0x00, '9', 0x00, 'D', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00 },
	},
};

uint8_t usbStringDescriptor0[] = {
	0x04,	// Size in bytes
	USB_STRING_DESCRIPTOR_TYPE,
	0x09, 0x04, // USB_STRING_LANID_ENGLISH
};

uint8_t usbStringDescManufacturer[] = {
	STR_DESC_SIZE(USB_STR_MANUFACTURER_LEN),
	USB_STRING_DESCRIPTOR_TYPE,
	USB_STR_MANUFACTURER,
};

uint8_t usbStringDescProduct[] = {
	STR_DESC_SIZE(USB_STR_PRODUCT_LEN),
	USB_STRING_DESCRIPTOR_TYPE,
	USB_STR_PRODUCT,
};

uint8_t usbStringDescSerialNumber[] = {
	STR_DESC_SIZE(USB_STR_SERIAL_NUM_LEN),
	USB_STRING_DESCRIPTOR_TYPE,
	USB_STR_SERIAL_NUM,
};