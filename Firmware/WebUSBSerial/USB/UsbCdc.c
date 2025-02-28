#include "UsbCdc.h"
#include "UsbDevice.h"

static UsbCdcLineCoding usbCdcLineCoding;

void UsbCdcInit()
{
	UsbCdcReset();
}

void UsbCdcReset()
{	
	usbCdcLineCoding = (UsbCdcLineCoding) { 115200, USB_CDC_CHAR_FORMAT_1_STOPBIT, USB_CDC_PARITY_NONE, 8 };
}

void UsbCdcSetLineCoding(uint8_t *data, uint32_t size)
{
	if(size == sizeof(UsbCdcLineCoding))
	{
		UsbCdcLineCoding *lc = (UsbCdcLineCoding *) data;
		usbCdcLineCoding = *lc;
	}
	
}

UsbCdcLineCoding *GetLineCoding()
{
	return &usbCdcLineCoding;
}