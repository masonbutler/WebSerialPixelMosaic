#include <stddef.h>
#include <stdbool.h>
#include "WebUsbSerialApp.h"
#include "GPIO.h"
#include "USB/UsbDevice.h"
#include "USB/UsbCdc.h"
#include "USB/UsbDescriptors.h"
#include "SPI.h"
#include "Leds.h"

sGPIOPin ledData = {
	GPIO_PORT_A,
	16,
	GPIO_PIN_MODE_OUTPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_C
};

sSPIDevice spiLeds = {
	SERCOM1,
	1,
	NULL,
	3000000U,
	SPI_OPERATING_MODE_HOST,
	SPI_DATA_ORDER_MSB,
	SPI_CLOCK_POL_IDLE_LOW,
	SPI_CLOCK_PHA_SAMPLE_LEADING,
	SPI_DATAIN_PAD3,
	SPI_DATAOUT_PAD_CONFIG0,
	SPI_DUPLEX_FULL,
};
			
sGPIOPin UsbDM = {
	GPIO_PORT_A,
	24,
	GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_G
};

sGPIOPin UsbDP = {
	GPIO_PORT_A,
	25,
	GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_G
};

sUsbDevice usbDevice = {
	USB,
	USB_SPEED_FULLSPEED
};

typedef enum {
	DATA_COMMAND_SET_PIXEL = 0x01,
	DATA_COMMAND_UPDATE_PIXEL = 0x02,
	DATA_COMMAND_UPDATE_PIXEL_ALL = 0x03,
	DATA_COMMAND_SET_PIXELS_DEFAULT_COLOR = 0x04,
} DataCommands;


// Callbacks
static void AppUsbDeviceConfig(uint8_t configNum);
static void AppHandleReceiveData(uint8_t endpoint_number, uint16_t size);
static void HandleCommand(uint8_t cmd, uint8_t *buff);

static void SetPixel(uint8_t num, uint8_t red, uint8_t green, uint8_t blue);
static void UpdatePixel(uint8_t num);
static void UpdatePixelAll();

static uint8_t receiveDataBuff[64];

void WebSerialAppInit()
{
	GPIOPinInit(&ledData);
	SPIInit(&spiLeds);
	
	GPIOPinInit(&UsbDM);
	GPIOPinInit(&UsbDP);
		
	UsbInit(&usbDevice);
	UsbCdcInit();
	
	usbDevice.callbacks.SetDeviceConfiguration = &AppUsbDeviceConfig;
	usbDevice.callbacks.OutTransferReceived = &AppHandleReceiveData;
	
	SPIEnable(&spiLeds);
	UsbEnable(&usbDevice);
	
	LedsInit();
	LedsDMAInit(&spiLeds);
	UsbConnect(&usbDevice);
	LedSendDataDMA();
}

static void AppUsbDeviceConfig(uint8_t configNum)
{
	UsbEndpointConfigure(&usbDevice, (UsbEndpointDescriptor *) &usbCombinedDescriptor.usbEpNotificationDescriptor);
	UsbEndpointConfigure(&usbDevice, (UsbEndpointDescriptor *) &usbCombinedDescriptor.usbEpCommInDescriptor);
	UsbEndpointConfigure(&usbDevice, (UsbEndpointDescriptor *) &usbCombinedDescriptor.usbEpCommOutDescriptor);
	UsbReceive(&usbDevice, USB_EP_CDC_RECEIVE, receiveDataBuff, sizeof(receiveDataBuff));
}

static void AppHandleReceiveData(uint8_t endpointNum, uint16_t size)
{
	if(endpointNum == USB_EP_CDC_RECEIVE)
	{
		HandleCommand(receiveDataBuff[0], &receiveDataBuff[1]);
	}
}

static void HandleCommand(uint8_t cmd, uint8_t *buff)
{
	switch(cmd)
	{
	case DATA_COMMAND_SET_PIXEL:
		SetPixel(buff[0], buff[1], buff[2], buff[3]);
		break;
	case DATA_COMMAND_UPDATE_PIXEL:
		UpdatePixel(buff[0]);
		break;
	case DATA_COMMAND_UPDATE_PIXEL_ALL:
		UpdatePixelAll();
		break;
	case DATA_COMMAND_SET_PIXELS_DEFAULT_COLOR:
		LedResetAllToDefault();
		break;
	}
	
	UsbReceive(&usbDevice, USB_EP_CDC_RECEIVE, receiveDataBuff, sizeof(receiveDataBuff));
}

static void SetPixel(uint8_t num, uint8_t red, uint8_t green, uint8_t blue)
{
	if(num >= 0 && num < NUM_LEDS)
	{
		LedSetColor(num, (sRGBColor){red, green, blue});
	}
}

static void UpdatePixel(uint8_t num)
{
	LedUpdateColorData(num);
	LedSendDataDMA(&spiLeds);
}

static void UpdatePixelAll()
{
	LedUpdateColorDataAll();
	LedSendDataDMA(&spiLeds);
}



