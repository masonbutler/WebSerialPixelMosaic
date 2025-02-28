#include <sam.h>
#include <string.h>
#include <stdalign.h>
#include "UsbDevice.h"
#include "UsbRequests.h"
#include "Standard/UsbStandard.h"
#include "UsbDescriptors.h"
#include "UsbRequests.h"
#include "../NVM_OTP.h"
#include "../Utility.h"
#include <assert.h>

/* Some sections of code (setting appropriate registers and setting up the USB periph) have been adapted
 * from Microchip's ASF and a bit from 
 * Alex Taradov's repo for the ATSAMD21: https://github.com/ataradov/dgw/blob/master/embedded/udc.c
 */

#define EP_TYPE_MASK	(0x3)

static sEndpointDMATransferDescriptor epTransDescr[NUM_EP_DMA_BANKS] __attribute__((aligned(4)));

static uint8_t ctrlEndpointOutBuf[64] __attribute__((aligned(4)));
static uint8_t ctrlEndpointInBuf[64] __attribute__((aligned(4)));

static void ConfigureClock(sUsbDevice *usbDevice);
static void EndpointReset(sUsbDevice *usbDevice, uint8_t endpointNum, USB_EP_DIR direction);
static void EndpointResetAll(sUsbDevice *usbDevice);
static void CallbacksReset(UsbEventCallbacks *callbacks);
static void EndpointConfigureControl0(sUsbDevice *usbDevice, UsbDeviceStandardDescriptor *deviceDesc);

// We need this because of the USB IRQ handler
static sUsbDevice *gCurrentDevice = NULL;

void UsbInit(sUsbDevice *usbDevice)
{
	Usb *module = usbDevice->usb;

	gCurrentDevice = usbDevice;
	ConfigureClock(usbDevice);
	
	memset(epTransDescr, 0, sizeof(sEndpointDMATransferDescriptor) * NUM_EP_DMA_BANKS);
	
	// Set USB transfer descriptor location
	module->DEVICE.DESCADD.reg = (uint32_t) epTransDescr;
	module->DEVICE.CTRLA.reg = (USB_CTRLA_MODE_DEVICE_Val << USB_CTRLA_MODE_Pos);
	module->DEVICE.CTRLA.bit.RUNSTDBY = 1;

	module->DEVICE.CTRLB.reg = (usbDevice->usbSpeedMode << USB_DEVICE_CTRLB_SPDCONF_Pos) | (0x1U << USB_DEVICE_CTRLB_DETACH_Pos);
	// Enable end of reset interrupt
	module->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_EORST;
	module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPINTENSET.bit.RXSTP = 1;
	
	CallbacksReset(&usbDevice->callbacks);
	EndpointResetAll(usbDevice);
}

void UsbEnable(sUsbDevice *usbDevice)
{
	Usb *module = usbDevice->usb;
	module->DEVICE.CTRLA.bit.ENABLE = 1;
	NVIC_EnableIRQ(USB_IRQn);
}

void UsbDisable(sUsbDevice *usbDevice)
{
	Usb *module = usbDevice->usb;
	module->DEVICE.CTRLA.bit.ENABLE = 0;
	NVIC_DisableIRQ(USB_IRQn);
}

void UsbConnect(sUsbDevice *usbDevice)
{
	Usb *module = usbDevice->usb;
	module->DEVICE.CTRLB.bit.DETACH = 0;
}

void UsbDisconnect(sUsbDevice *usbDevice)
{
	Usb *module = usbDevice->usb;
	module->DEVICE.CTRLB.bit.DETACH = 1;
}

void UsbEndpointConfigure(sUsbDevice *usbDevice, UsbEndpointDescriptor *ep)
{
	Usb *module = usbDevice->usb;
	uint8_t endpointNum = ep->bEndpointAddress & USB_EP_NUM_MSK;
	uint8_t type = 0;
	uint16_t size = 0;
	
	if((ep->bmAttributes & USB_EP_TYPE_MSK) == USB_ENDPOINT_TYPE_CONTROL)
		type = USB_DEVICE_SPEC_EPTYPE_CONTROL;
	else if((ep->bmAttributes & USB_EP_TYPE_MSK) == USB_ENDPOINT_TYPE_ISOCHRONOUS)
		type = USB_DEVICE_SPEC_EPTYPE_ISOCHRONOUS;
	else if((ep->bmAttributes & USB_EP_TYPE_MSK) == USB_ENDPOINT_TYPE_BULK)
		type = USB_DEVICE_SPEC_EPTYPE_BULK;
	else if((ep->bmAttributes & USB_EP_TYPE_MSK) == USB_ENDPOINT_TYPE_INTERRUPT)
		type = USB_DEVICE_SPEC_EPTYPE_INTERRUPT;
	else
		assert("Invalid EP type!");
	
	if(ep->wMaxPacketSize == 8)
		size = USB_EP_MAX_PACKET_SIZE_8;
	else if(ep->wMaxPacketSize == 16)
		size = USB_EP_MAX_PACKET_SIZE_16;
	else if(ep->wMaxPacketSize == 32)
		size = USB_EP_MAX_PACKET_SIZE_32;
	else if(ep->wMaxPacketSize == 64)
		size = USB_EP_MAX_PACKET_SIZE_64;
	else if(ep->wMaxPacketSize == 128)
		size = USB_EP_MAX_PACKET_SIZE_128;
	else if(ep->wMaxPacketSize == 256)
		size = USB_EP_MAX_PACKET_SIZE_256;
	else if(ep->wMaxPacketSize == 512)
		size = USB_EP_MAX_PACKET_SIZE_512;
	else if(ep->wMaxPacketSize == 1024)
		size = USB_EP_MAX_PACKET_SIZE_1024;
	else
		assert("Invalid EP packet size!");
	
	
	if(ep->bEndpointAddress & USB_EP_DIR_MSK)	// In
	{
		module->DEVICE.DeviceEndpoint[endpointNum].EPCFG.bit.EPTYPE1 = type;
		module->DEVICE.DeviceEndpoint[endpointNum].EPINTENSET.bit.TRCPT1 = 1;
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSCLR.bit.DTGLIN = 1;
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSCLR.bit.BK1RDY = 1;
		epTransDescr[endpointNum].in.PCKSIZE.bit.SIZE = size;
	}
	else // Out
	{
		module->DEVICE.DeviceEndpoint[endpointNum].EPCFG.bit.EPTYPE0 = type;
		module->DEVICE.DeviceEndpoint[endpointNum].EPINTENSET.bit.TRCPT0 = 1;
		// Enable toggle of DATA identifier for out packets
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSCLR.bit.DTGLOUT = 1;
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.BK0RDY = 1;
		epTransDescr[endpointNum].out.PCKSIZE.bit.SIZE = size;
	}
}

void UsbSetAddress(sUsbDevice *usbDevice, uint8_t address)
{
	Usb *module = usbDevice->usb;
	module->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | USB_DEVICE_DADD_DADD(address);
}

void UsbSendZLP(sUsbDevice *usbDevice, uint8_t endpointNum)
{
	Usb *module = usbDevice->usb;
	
	epTransDescr[endpointNum].in.PCKSIZE.bit.BYTE_COUNT = 0;
	module->DEVICE.DeviceEndpoint[endpointNum].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
	module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.BK1RDY = 1;
	
	while(!module->DEVICE.DeviceEndpoint[endpointNum].EPINTFLAG.bit.TRCPT1);
}

void UsbEndpointStall(sUsbDevice *usbDevice, uint8_t endpointNum, USB_EP_DIR direction)
{
	Usb *module = usbDevice->usb;

	if (direction == USB_EP_DIR_OUT)
	{
		// Stall out EP
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.STALLRQ0 = 1;
	}
	else
	{
		// Stall in EP
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.STALLRQ1 = 1;
	}
}

bool UsbGetEndpointStall(sUsbDevice *usbDevice, uint8_t endpointNum, USB_EP_DIR direction)
{
	Usb *module = usbDevice->usb;
	
	if (direction == USB_EP_DIR_OUT)
	{
		return module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.STALLRQ0;
	}
	else
	{
		return module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.STALLRQ1;
	}
	
	return false;
}

void UsbSend(sUsbDevice *usbDevice, uint8_t endpointNum, uint8_t *data, uint32_t size)
{
	Usb *module = usbDevice->usb;
	uint32_t bytesLeft = size;
	
	if(endpointNum == CONTROL_EP_NUM)
	{
		while (bytesLeft)
		{
			int sendSize = MIN(bytesLeft, usbDeviceDescriptor.bMaxPacketSize0);
			
			for (int i = 0; i < sendSize; i++)
				ctrlEndpointInBuf[i] = data[i];

			epTransDescr[CONTROL_EP_NUM].in.ADDR.reg = (uint32_t)ctrlEndpointInBuf;
			epTransDescr[CONTROL_EP_NUM].in.PCKSIZE.bit.BYTE_COUNT = sendSize;
			epTransDescr[CONTROL_EP_NUM].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
			
			module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
			module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPSTATUSSET.bit.BK1RDY = 1;

			while (!module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPINTFLAG.bit.TRCPT1);

			bytesLeft -= sendSize;
			data += sendSize;
		}
	}
	else
	{
		epTransDescr[endpointNum].in.ADDR.reg = (uint32_t) data;
		epTransDescr[endpointNum].in.PCKSIZE.bit.BYTE_COUNT = size;
		epTransDescr[endpointNum].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
		epTransDescr[endpointNum].in.PCKSIZE.bit.AUTO_ZLP = 1;
		// Set in endpoint ready to transmit
		module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSSET.bit.BK1RDY = 1;
	}
}

void UsbReceive(sUsbDevice *usbDevice, uint8_t endpointNum, uint8_t *data, uint32_t size)
{
	Usb *module = usbDevice->usb;

	epTransDescr[endpointNum].out.ADDR.reg = (uint32_t) data;
	epTransDescr[endpointNum].out.PCKSIZE.bit.MULTI_PACKET_SIZE = size;
	epTransDescr[endpointNum].out.PCKSIZE.bit.BYTE_COUNT = 0;

	module->DEVICE.DeviceEndpoint[endpointNum].EPSTATUSCLR.bit.BK0RDY = 1;
}

static void ConfigureClock(sUsbDevice *usbDevice)
{
	Usb *module = usbDevice->usb;
	
	uint32_t transn = *((uint32_t *)(NVMCTRL_OTP5)) >> NVM_OTP_USB_TRANSN_POS;
	uint32_t transp = *((uint32_t *)(NVMCTRL_OTP5)) >> NVM_OTP_USB_TRANSP_POS;
	uint32_t trim = *((uint32_t *)(NVMCTRL_OTP5)) >> NVM_OTP_USB_TRIM_POS;
	
	MCLK->APBBMASK.reg |= MCLK_APBBMASK_USB;
	GCLK->PCHCTRL[USB_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0) | GCLK_PCHCTRL_CHEN;
	while (!(GCLK->PCHCTRL[USB_GCLK_ID].reg & GCLK_PCHCTRL_CHEN));

	module->DEVICE.CTRLA.reg = USB_CTRLA_SWRST;
	while(module->DEVICE.SYNCBUSY.reg & USB_SYNCBUSY_SWRST);

	module->DEVICE.PADCAL.reg = (USB_PADCAL_TRANSN(transn) | USB_PADCAL_TRANSP(transp) | USB_PADCAL_TRIM(trim));
}

void EndpointReset(sUsbDevice *usbDevice, uint8_t endpointNum, USB_EP_DIR direction)
{
	Usb *module = usbDevice->usb;
	
	if (direction == USB_EP_DIR_OUT)
	{
		// Disable out endpoint
		module->DEVICE.DeviceEndpoint[endpointNum].EPCFG.bit.EPTYPE0 = USB_DEVICE_SPEC_EPTYPE_DISABLED;
	}
	else
	{
		// Disable in endpoint
		module->DEVICE.DeviceEndpoint[endpointNum].EPCFG.bit.EPTYPE1 = USB_DEVICE_SPEC_EPTYPE_DISABLED;
	}
}

static void EndpointResetAll(sUsbDevice *usbDevice)
{
	for (int i = 0; i < USB_NUM_ENDPOINTS; i++)
	{
		EndpointReset(usbDevice, i, USB_EP_DIR_OUT);
		EndpointReset(usbDevice, i, USB_EP_DIR_IN);
	}
}

static void CallbacksReset(UsbEventCallbacks *callbacks)
{
	callbacks->SetDeviceConfiguration = NULL;
	//callbacks->inTransferCompleted = NULL;
	callbacks->OutTransferReceived = NULL;
	//callbacks->outTransferCompleted = NULL;
}

static void EndpointConfigureControl0(sUsbDevice *usbDevice, UsbDeviceStandardDescriptor *deviceDesc)
{
	Usb *module = usbDevice->usb;
	
	uint8_t size = 0;
	
	if(deviceDesc->bMaxPacketSize0 == 8)
		size = USB_EP_MAX_PACKET_SIZE_8;
	else if(deviceDesc->bMaxPacketSize0 == 16)
		size = USB_EP_MAX_PACKET_SIZE_16;
	else if(deviceDesc->bMaxPacketSize0 == 32)
		size = USB_EP_MAX_PACKET_SIZE_32;
	else if(deviceDesc->bMaxPacketSize0 == 64)
		size = USB_EP_MAX_PACKET_SIZE_64;
	else
		assert("Invalid packet size");
	
	module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(USB_DEVICE_SPEC_EPTYPE_CONTROL)
															| USB_DEVICE_EPCFG_EPTYPE1(USB_DEVICE_SPEC_EPTYPE_CONTROL);
	// Set in EP ready to receive and out endpoint to not ready
	module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPSTATUSSET.bit.BK0RDY = 1;
	module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPSTATUSCLR.bit.BK1RDY = 1;

	//  Configure out endpoint buffer and packet info
	epTransDescr[CONTROL_EP_NUM].out.ADDR.reg = (uint32_t) ctrlEndpointOutBuf;
	epTransDescr[CONTROL_EP_NUM].out.PCKSIZE.bit.SIZE = size;
	epTransDescr[CONTROL_EP_NUM].out.PCKSIZE.bit.MULTI_PACKET_SIZE = 64;
	epTransDescr[CONTROL_EP_NUM].out.PCKSIZE.bit.BYTE_COUNT = 0;

	//  Configure in endpoint buffer and packet info
	epTransDescr[CONTROL_EP_NUM].in.ADDR.reg = (uint32_t) ctrlEndpointInBuf;
	epTransDescr[CONTROL_EP_NUM].in.PCKSIZE.bit.SIZE = size;
	epTransDescr[CONTROL_EP_NUM].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
	epTransDescr[CONTROL_EP_NUM].in.PCKSIZE.bit.BYTE_COUNT = 0;
	
	// Enable endpoint received setup interrupt
	module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPINTENSET.bit.RXSTP = 1;
	module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPINTENSET.bit.TRCPT0 = 1;
}


void UsbHandler()
{
	Usb *module = gCurrentDevice->usb;

	// If end of reset interrupt is set
	if (module->DEVICE.INTFLAG.bit.EORST)
	{
		// Set bit to clear the flag
		module->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
		// Enable adding the device address
		module->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN;
		
		EndpointResetAll(gCurrentDevice);
		EndpointConfigureControl0(gCurrentDevice, &usbDeviceDescriptor);
	}
	

	// If received setup packet interrupt is set
	if (module->DEVICE.DeviceEndpoint[CONTROL_EP_NUM].EPINTFLAG.bit.RXSTP)
	{
		if(epTransDescr[CONTROL_EP_NUM].out.PCKSIZE.bit.BYTE_COUNT == sizeof(UsbRequest))
		{
			// Handle the USB request
			UsbHandleRequest(gCurrentDevice, (UsbRequest *) ctrlEndpointOutBuf);
			epTransDescr[0].out.PCKSIZE.bit.BYTE_COUNT = 0;
			module->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.bit.BK0RDY = 1;
			module->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
		}
		else
		{
			UsbEndpointStall(gCurrentDevice, CONTROL_EP_NUM, USB_EP_DIR_IN);
		}
		
		// Set bit to clear flag
		module->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP;

	}
	else if (module->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT0)
	{
		// EP0 receive for non-control data 
	}

	uint32_t endpointInterrupts = module->DEVICE.EPINTSMRY.reg;
	
	for (int i = 1; (endpointInterrupts > 0) && (i < USB_NUM_ENDPOINTS); i++)
	{	
		// Look through flags
		uint32_t flags = module->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg;		
		// Clear flag
		endpointInterrupts &= ~(1U << i);
		
		if (flags & USB_DEVICE_EPINTFLAG_TRCPT0)
		{
			module->DEVICE.DeviceEndpoint[i].EPSTATUSSET.bit.BK0RDY = 1;
			module->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
			
			if(gCurrentDevice->callbacks.OutTransferReceived != NULL)
			{
				gCurrentDevice->callbacks.OutTransferReceived(i, epTransDescr[i].out.PCKSIZE.bit.BYTE_COUNT);
				
			}
		}

		if (flags & USB_DEVICE_EPINTFLAG_TRCPT1)
		{
			module->DEVICE.DeviceEndpoint[i].EPSTATUSCLR.bit.BK1RDY = 1;
			module->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg |= USB_DEVICE_EPINTFLAG_TRCPT1;
			
		}
	}
}