#include <stdint.h>
#include "sam.h"
#include "UART.h"
#include "System.h"
#include "GPIO.h"
#include "SERCOM.h"

static void ConfigurePads(sUARTDevice *device);
static void ConfigureTransceiver(sUARTDevice *device);
static void ConfigureCommsBits(sUARTDevice *device);
static uint32_t CalculateBaud(uint32_t baud);
static void UARTPutChar(Sercom *module, char c);

void UARTInit(sUARTDevice *device, uint8_t sercomNumber)
{
	SercomClockEnable(sercomNumber);
	
	Sercom *module = device->sercomDevice;
	
	module->USART.CTRLA.bit.SWRST = 1;
	while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_SWRST);
	
	// Set to run on internal clock
	module->USART.CTRLA.reg |= SERCOM_USART_CTRLA_MODE(0x1);
	ConfigurePads(device);
	ConfigureTransceiver(device);
	ConfigureCommsBits(device);
	
	module->USART.BAUD.reg = (uint16_t) CalculateBaud(device->baud);
}

void UARTEnable(sUARTDevice *device)
{
	Sercom *module = device->sercomDevice;
	
	module->USART.CTRLA.bit.ENABLE = 1;
	while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}

void UARTDisable(sUARTDevice *device)
{
	Sercom *module = device->sercomDevice;
	
	module->USART.CTRLA.bit.ENABLE = 0;
	while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}

void UARTWrite(sUARTDevice *device, char *string)
{
	while(*string)
	{
		UARTPutChar(device->sercomDevice, *string++);
	}	
}

uint8_t UARTRead(sUARTDevice *device)
{
	Sercom *module = device->sercomDevice;
	while(!(module->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC));
	
	return module->USART.DATA.reg;
}

static void UARTPutChar(Sercom *module, char c)
{
	while(!(module->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE));
	module->USART.DATA.reg = c;
}

static void ConfigurePads(sUARTDevice *device)
{
	Sercom *module = device->sercomDevice;

	module->USART.CTRLA.reg |= SERCOM_USART_CTRLA_RXPO(device->rxPadPos);
	module->USART.CTRLA.reg |= SERCOM_USART_CTRLA_TXPO(device->txPadPos);
}

static void ConfigureTransceiver(sUARTDevice *device)
{
	Sercom *module = device->sercomDevice;
		
	if(device->transceiverEnable & UART_TRANSCEIVER_ENABLE_TX)
	{
		module->USART.CTRLB.bit.TXEN = 1;
		while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_CTRLB);
	}

	if(device->transceiverEnable & UART_TRANSCEIVER_ENABLE_RX)
	{
		module->USART.CTRLB.bit.RXEN = 1;
		while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_CTRLB);
	}
}

static void ConfigureCommsBits(sUARTDevice *device)
{
	Sercom *module = device->sercomDevice;
	
	// Transmit LSB first
	module->USART.CTRLA.bit.DORD = 1;
	
	module->USART.CTRLB.reg |= (device->parity >> SERCOM_USART_CTRLB_PMODE_Pos) & SERCOM_USART_CTRLB_PMODE;
	while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_CTRLB);
	
	module->USART.CTRLB.reg |= (device->numStopBits >> SERCOM_USART_CTRLB_SBMODE_Pos) & SERCOM_USART_CTRLB_SBMODE;
	while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_CTRLB);
	
	module->USART.CTRLB.reg |= (device->charSize >> SERCOM_USART_CTRLB_CHSIZE_Pos) & SERCOM_USART_CTRLB_CHSIZE_Msk;
	while(module->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_CTRLB);
}

static uint32_t CalculateBaud(uint32_t baud)
{
	return ((uint64_t) 65536 * (SYS_CLK - 16 * baud) / SYS_CLK);
}