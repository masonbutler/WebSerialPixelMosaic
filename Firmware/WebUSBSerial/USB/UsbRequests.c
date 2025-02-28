#include "sam.h"
#include "UsbRequests.h"
#include "UsbDescriptors.h"
#include "Standard/UsbCdcStandard.h"
#include "UsbCdc.h"
#include "../Utility.h"
#include <stddef.h>

static void HandleStandardRequest(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleStandardInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleClassInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleVendorDeviceRequest(sUsbDevice *usbDevice, UsbRequest *request);

static void HandleGetDescriptor(sUsbDevice *usbDevice, UsbRequest *request);
static void HandleStringDescriptors(sUsbDevice *usbDevice, uint8_t index);

void UsbHandleRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	// Check request type then call the correct function on class request type
	switch(request->bmRequestType & (USB_BM_REQUEST_TYPE_TYPE_MASK | USB_BM_REQUEST_TYPE_RECIPIENT_MASK))
	{
	case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE:
		HandleStandardRequest(usbDevice, request);
		break;
	case USB_BM_REQUEST_TYPE_TYPE_CLASS | USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE:
		HandleClassInterfaceRequest(usbDevice, request);
		break;
	case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE:
		HandleStandardInterfaceRequest(usbDevice, request);
		break;
	case USB_BM_REQUEST_TYPE_TYPE_VENDOR | USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE:
		HandleVendorDeviceRequest(usbDevice, request);
		break;
	default:
		UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		break;
	}
}

static void HandleStandardRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	//uint16_t length = request->wLength;
	uint8_t value = request->wValue >> 8;

	switch(request->bRequest)
	{
	case USB_STANDARD_DEVICE_REQUEST_GET_DESCRIPTOR:
		HandleGetDescriptor(usbDevice, request);
		break;
	case USB_STANDARD_DEVICE_REQUEST_SET_ADDRESS:
		UsbSendZLP(usbDevice, CONTROL_EP_NUM);
		UsbSetAddress(usbDevice, request->wValue);
		break;	
	case USB_STANDARD_DEVICE_REQUEST_SET_CONFIGURATION:
		UsbSendZLP(usbDevice, CONTROL_EP_NUM);
		
		if(usbDevice->callbacks.SetDeviceConfiguration != NULL)
		{
			usbDevice->callbacks.SetDeviceConfiguration(value);
		}

		break;
	default:
		UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		break;
	}
}

static void HandleClassInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	uint16_t length = request->wLength;
	//uint8_t value = request->wValue >> 8;
	
	switch(request->bRequest)
	{
	case USB_CDC_REQUESTS_SET_LINE_CODING:
		{
			uint8_t buf[sizeof(UsbCdcLineCoding)];
			length = MIN(length, sizeof(UsbCdcLineCoding));
			UsbReceive(usbDevice, CONTROL_EP_NUM, buf, length);
			UsbCdcSetLineCoding(buf, sizeof(UsbCdcLineCoding));
			UsbSendZLP(usbDevice, CONTROL_EP_NUM);
		}
		break;
	case USB_CDC_REQUESTS_GET_LINE_CODING:
		{
			UsbCdcLineCoding *lc = GetLineCoding();
			length = MIN(length, sizeof(UsbCdcLineCoding));
			UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *) lc, length);
		}
		break;
	case USB_CDC_REQUESTS_SET_CONTROL_LINE_STATE:
		UsbSendZLP(usbDevice, CONTROL_EP_NUM);
		break;
	 default:
		UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		break;
	}
}

static void HandleStandardInterfaceRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
}

static void HandleVendorDeviceRequest(sUsbDevice *usbDevice, UsbRequest *request)
{
	//uint8_t descType = request->wValue >> 8;
	uint16_t length = request->wLength;
	
	switch(request->bRequest)
	{
	case MS_OS20_VENDOR_REQUEST_DESCRIPTOR:
		length = MIN(length, usbMsDescriptor.usbMsSetHeaderDescriptor.wTotalLength);
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)&usbMsDescriptor, length);
		break;
	default:
		UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		break;
	}
}

static void HandleGetDescriptor(sUsbDevice *usbDevice, UsbRequest *request)
{
	uint8_t descType = request->wValue >> 8;
	uint16_t length = request->wLength;
	
	switch(descType)
	{
	case USB_DESCRIPTOR_TYPE_DEVICE:
		length = MIN(length, usbDeviceDescriptor.bLength);
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)&usbDeviceDescriptor, length);
		break;
	case USB_DESCRIPTOR_TYPE_CONFIGURATION:
		length = MIN(length, usbCombinedDescriptor.usbConfigurationDescriptor.wTotalLength);
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)&usbCombinedDescriptor, length);
		break;
	case USB_DESCRIPTOR_TYPE_STRING:
		{
			uint8_t stringIndex = request->wValue & 0xFF;
			HandleStringDescriptors(usbDevice, stringIndex);
		}
		break;
	case USB_DESCRIPTOR_TYPE_BOS:
		length = MIN(length, usbBosCombinedDescriptor.usbBosDescriptor.wTotalLength);
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)&usbBosCombinedDescriptor, length);
		break;
	default:
		UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		break;
	}
}

static void HandleStringDescriptors(sUsbDevice *usbDevice, uint8_t index)
{
	switch(index)
	{
	case USB_STRING_DESCRIPTOR_INDEX_0:
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescriptor0, sizeof(usbStringDescriptor0));
		break;
	case USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER:
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescManufacturer, sizeof(usbStringDescManufacturer));
		break;
	case USB_STRING_DESCRIPTOR_INDEX_PRODUCT:
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescProduct, sizeof(usbStringDescProduct));
		break;
	case USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUM:
		UsbSend(usbDevice, CONTROL_EP_NUM, (uint8_t *)usbStringDescSerialNumber, sizeof(usbStringDescSerialNumber));
		break;
	default:
		// Stall since we don't handle other requests
		UsbEndpointStall(usbDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		break;
	}
}