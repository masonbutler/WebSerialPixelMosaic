#ifndef USBCDCSTANDARD_H_
#define USBCDCSTANDARD_H_
#include "UsbStandard.h"

#define USB_CDC_DATA_INTERFACE_CLASS	0xA

typedef enum {
	USB_CDC_SUBCLASS_DIRECT_LINE_CONTROL_MODE = 0x01,
	USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,
	USB_CDC_SUBCLASS_TELEPHONE_CONTROL_MODEL,
	USB_CDC_SUBCLASS_MULTICHANNEL_CONTROL_MODEL,
	USB_CDC_SUBCLASS_CAPI_CONTROL_MODEL,
	USB_CDC_SUBCLASS_ETHERNET_NETWORKING_CONTROL_MODEL,
	USB_CDC_SUBCLASS_ATM_NETWORKING_CONTROL_MODEL,
	USB_CDC_SUBCLASS_WIRELESS_HANDSET_CONTROL_MODEL,
	USB_CDC_SUBCLASS_DEVICE_MANAGEMENT,
	USB_CDC_SUBCLASS_MOBILE_DIRECT_LINE_MODEL,
	USB_CDC_SUBCLASS_OBEX,
	USB_CDC_SUBCLASS_ETHERNET_EMULATION_MODEL,
	USB_CDC_SUBCLASS_NETWORK_CONTROL_MODEL
} USB_CDC_SUBCLASS;

typedef enum {
	USB_CDC_CLASS_PROTOCOL_NO_PROTOCOL = 0x00,
	USB_CDC_CLASS_PROTOCOL_VENDOR_SPECIFIC = 0xFF
} USB_CDC_CLASS_PROTOCOL;

typedef enum {
	USB_CDC_INTERFACE_PROTOCOL_NO_PROTOCOL = 0x00,
	USB_CDC_INTERFACE_PROTOCOL_IN_FUNCTIONAL_DESCRIPTOR = 0xFE,
	USB_CDC_INTERFACE_PROTOCOL_VENDOR_SPECIFIC = 0xFF,
} USB_CDC_INTERFACE_PROTOCOL;

typedef enum {
	USB_CDC_SUBTYPE_HEADER = 0x00,
	USB_CDC_SUBTYPE_CALL_MANAGEMENT,
	USB_CDC_SUBTYPE_ABSTRACT_CONTROL_MANAGEMENT,
	USB_CDC_SUBTYPE_UNION = 0x06
} USB_CDC_SUBTYPE;

typedef enum
{
	USB_CDC_REQUESTS_SEND_ENCAPSULATED_COMMAND = 0x00,
	USB_CDC_REQUESTS_GET_ENCAPSULATED_RESPONSE,
	USB_CDC_REQUESTS_SET_COMM_FEATURE,
	USB_CDC_REQUESTS_GET_COMM_FEATURE,
	USB_CDC_REQUESTS_CLEAR_COMM_FEATURE,
	USB_CDC_REQUESTS_SET_LINE_CODING = 0x20,
	USB_CDC_REQUESTS_GET_LINE_CODING,
	USB_CDC_REQUESTS_SET_CONTROL_LINE_STATE,
	USB_CDC_REQUESTS_SEND_BREAK,
} USB_CDC_REQUESTS;

typedef enum {
	USB_CDC_CHAR_FORMAT_1_STOPBIT = 0x00,
	USB_CDC_CHAR_FORMAT_1_5_STOPBIT,
	USB_CDC_CHAR_FORMAT_2_STOPBITS
} USB_CDC_CHAR_FORMAT;

typedef enum {
	USB_CDC_PARITY_NONE = 0x00,
	USB_CDC_PARITY_ODD,
	USB_CDC_PARITY_EVEN,
	USB_CDC_PARITY_MARK,
	USB_CDC_PARITY_SPACE,
} USB_CDC_PARITY;

typedef enum {
	USB_CDC_ACM_SUPPORT_FEATURE_REQUESTS = 0x01,
	USB_CDC_ACM_SUPPORT_LINE_REQUESTS = 0x02,
	USB_CDC_ACM_SUPPORT_SENDBREAK_REQUESTS = 0x04,
	USB_CDC_ACM_SUPPORT_NOTIFICATION_REQUESTS = 0x08,
} USB_CDC_ACM_SUPPORT;


typedef enum {
	USB_CDC_CALL_MANAGEMENT_SUPPORT_SUPPORTED = 0x01,
	USB_CDC_CALL_MANAGEMENT_SUPPORT_OVER_DCI = 0x02,
} USB_CDC_CALL_MANAGEMENT_SUPPORT;

typedef enum {
	USB_CDC_NOTIFICATION_SERIAL_STATE       = 0x20,
	USB_CDC_NOTIFICATION_CALL_STATE_CHANGE  = 0x28,
	USB_CDC_NOTIFICATION_LINE_STATE_CHANGE  = 0x29,
	USB_CDC_NOTIFICATION_CONNECTION_SPEED_CHANGE = 0x2A,
} USB_CDC_NOTIFICATION;

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdCDC;
} __attribute__((__packed__)) UsbCdcHeaderFunctionalDescriptor;

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
} __attribute__((__packed__)) UsbCdcAbstractControlManagementDescriptor;

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} __attribute__((__packed__)) UsbCdcCallManagementFunctionalDescriptor;

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bControlInterface;
	uint8_t bSubordinateInferface0;
} __attribute__((__packed__)) UsbCdcUnionFunctionalDescriptor;

typedef struct {
	uint32_t dwDTERate;
	uint8_t bCharFormat;
	uint8_t bParityType;
	uint8_t bDataBits;
} __attribute__((__packed__)) UsbCdcLineCoding;

typedef struct {
	UsbRequest request;
	uint16_t value;
} __attribute__((__packed__)) UsbCdcNotificationSerialState;

typedef struct {
	uint32_t DLBitRate;
	uint32_t ULBitRate;
} __attribute__((__packed__)) UsbCdcNotificationConnectSpeedChange;

typedef struct {
	uint16_t DTEPresent : 1;
	uint16_t activeHalfDuplexCarrierControl : 1;
	uint16_t reserved : 14;
} __attribute__((__packed__)) UsbCdcControlSignalBitmap;

typedef struct {
	uint16_t bRxCarrier : 1;
	uint16_t bTxCarrier : 1;
	uint16_t bBreak : 1;
	uint16_t bRingSignal : 1;
	uint16_t bFraming : 1;
	uint16_t bParity: 1;
	uint16_t bOverRun : 1;
	uint16_t reserved : 9;
} __attribute__((__packed__)) UsbCdcUARTStateBitmap;

#endif