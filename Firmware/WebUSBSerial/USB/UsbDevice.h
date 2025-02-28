#ifndef USBDEVICE_H_
#define USBDEVICE_H_
#include <sam.h>
#include <stdbool.h>
#include "Standard/UsbStandard.h"

// Each endpoint is handled in IN and OUT pairs (i.e.  EP0 and EP1 = 2 EPs (with each having IN and OUT)
#define USB_NUM_ENDPOINTS	4
#define CONTROL_EP_NUM		0
#define EP_BANK_OUT			0
#define EP_BANK_IN			1

// The DMA is stored in banks of out and in. So we calculate how many banks we need
// We need to include EP0 in this calculations
#define NUM_EP_DMA_BANKS (4)

typedef enum {
	USB_SPEED_FULLSPEED = 0,
	USB_SPEED_LOWSPEED,
} USB_SPEED_MODE;

typedef enum  {
	USB_EP_DIR_OUT = 0,
	USB_EP_DIR_IN,
} USB_EP_DIR;

typedef enum {
	USB_EP_MAX_PACKET_SIZE_8 = 0,
	USB_EP_MAX_PACKET_SIZE_16,
	USB_EP_MAX_PACKET_SIZE_32,
	USB_EP_MAX_PACKET_SIZE_64,
	USB_EP_MAX_PACKET_SIZE_128,
	USB_EP_MAX_PACKET_SIZE_256,
	USB_EP_MAX_PACKET_SIZE_512,
	USB_EP_MAX_PACKET_SIZE_1024,
} USB_EP_MAX_PACKET_SIZE;

typedef enum
{
	USB_DEVICE_SPEC_EPTYPE_DISABLED = 0,
	USB_DEVICE_SPEC_EPTYPE_CONTROL,
	USB_DEVICE_SPEC_EPTYPE_ISOCHRONOUS,
	USB_DEVICE_SPEC_EPTYPE_BULK,
	USB_DEVICE_SPEC_EPTYPE_INTERRUPT,
} USB_DEVICE_SPEC_EPTYPE;

// Holds USB transfer information
typedef struct
{
	UsbDeviceDescBank out;
	UsbDeviceDescBank in;
} __attribute__((__packed__)) sEndpointDMATransferDescriptor;

typedef struct {
	void (*SetDeviceConfiguration)(uint8_t configNum);
	//void (*inTransferCompleted)(uint8_t endpoint_number);
	void (*OutTransferReceived)(uint8_t endpointNum, uint16_t size);
	//void (*outTransferCompleted)(uint8_t endpoint_number);
} UsbEventCallbacks;

typedef struct {
	Usb *usb;
	uint8_t usbSpeedMode;
	
	UsbEventCallbacks callbacks; 
} sUsbDevice;

void UsbInit(sUsbDevice *usbDevice);
void UsbEnable(sUsbDevice *usbDevice);
void UsbDisable(sUsbDevice *usbDevice);

void UsbConnect(sUsbDevice *usbDevice);
void UsbDisconnect(sUsbDevice *usbDevice);

void UsbEndpointConfigure(sUsbDevice *usbDevice, UsbEndpointDescriptor *ep);
void UsbSetAddress(sUsbDevice *usbDevice, uint8_t address);

void UsbSendZLP(sUsbDevice *usbDevice, uint8_t endpointNum);
void UsbEndpointStall(sUsbDevice *usbDevice, uint8_t endpointNum, USB_EP_DIR direction);
bool UsbGetEndpointStall(sUsbDevice *usbDevice, uint8_t endpointNum, USB_EP_DIR direction);

void UsbSend(sUsbDevice *usbDevice, uint8_t endpointNum, uint8_t *data, uint32_t size);
void UsbReceive(sUsbDevice *usbDevice, uint8_t endpointNum, uint8_t *data, uint32_t size);

void UsbHandler();

#endif