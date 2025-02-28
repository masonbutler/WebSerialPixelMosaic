#include <sam.h>
#include "SPI.h"
#include "System.h"
#include "GPIO.h"
#include "SERCOM.h"

static void ConfigureCommsSettings(sSPIDevice *device);
static void ConfigurePads(sSPIDevice *device);
static uint32_t ConfigureFrequency(uint32_t freq);

void SPIInit(sSPIDevice *device)
{
	SercomClockEnable(device->sercomNumber);
	
	Sercom *module = device->sercomDevice;
	module->SPI.CTRLA.bit.SWRST = 1;
	while(module->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST);
	

	ConfigureCommsSettings(device);
	ConfigurePads(device);
	
	module->SPI.BAUD.reg = ConfigureFrequency(device->clockSpeed);
}

void SPIEnable(sSPIDevice *device)
{
	Sercom *module = device->sercomDevice;
	module->SPI.CTRLA.bit.ENABLE = 1;
	while (module->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIDisable(sSPIDevice *device)
{
	Sercom *module = device->sercomDevice;
	module->SPI.CTRLA.bit.ENABLE = 0;
	while (module->SPI.SYNCBUSY.bit.ENABLE);	
}

void SPISetSS(sSPIDevice *device)
{
	sGPIOPin *ssPin = device->softwareSSPin;
	PORT->Group[ssPin->port].OUT.reg |= (0x1 << ssPin->pinNum);
}

void SPIClearSS(sSPIDevice *device)
{
	sGPIOPin *ssPin = device->softwareSSPin;
	PORT->Group[ssPin->port].OUT.reg &= ~(0x1 << ssPin->pinNum);
}

void SPIWriteBuffer(sSPIDevice *device, uint8_t *buffer, uint32_t size)
{
	Sercom *module = device->sercomDevice;

	uint32_t bytesRemaining = size;
	
	while(bytesRemaining > 0)
	{
		while (!module->SPI.INTFLAG.bit.DRE);
		module->SPI.DATA.reg = *buffer;
		
		buffer++;
		bytesRemaining--;
	}
}

uint8_t SPIReadByte(sSPIDevice *device)
{
	Sercom *module = device->sercomDevice;
	
	// Fake write to generate clock
	while (!module->SPI.INTFLAG.bit.DRE);
	module->SPI.DATA.reg = 0xFF;

	while (!module->SPI.INTFLAG.bit.RXC);

	return module->SPI.DATA.reg;
}


static void ConfigureCommsSettings(sSPIDevice *device)
{
	Sercom *module = device->sercomDevice;
	
	module->SPI.CTRLA.reg |= (device->dataOrder << SERCOM_SPI_CTRLA_DORD_Pos) & SERCOM_SPI_CTRLA_DORD;
	module->SPI.CTRLA.reg |= (device->clockPol << SERCOM_SPI_CTRLA_CPOL_Pos) & SERCOM_SPI_CTRLA_CPOL;	
	module->SPI.CTRLA.reg |= (device->clockPha << SERCOM_SPI_CTRLA_CPHA_Pos) & SERCOM_SPI_CTRLA_CPHA;
	module->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_MODE(device->operatingMode);
	
	module->SPI.CTRLB.reg |= (device->duplexMode << SERCOM_SPI_CTRLB_RXEN_Pos) & SERCOM_SPI_CTRLB_RXEN;	
	while(module->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_CTRLB);	
}

static void ConfigurePads(sSPIDevice *device)
{
	Sercom *module = device->sercomDevice;
	
	module->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_DOPO(device->dataOutPadConfig);
	module->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_DIPO(device->dataInPad);
}

static uint32_t ConfigureFrequency(uint32_t freq)
{
	return (SYS_CLK / (2 * freq)) - 1;
}