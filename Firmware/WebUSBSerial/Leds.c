#include <sam.h>
#include "Leds.h"
#include <stdbool.h>

#define RESET_SIGNAL_LENGTH		50
#define NUM_BITS_PER_LED		24
#define LED_BYTESTREAM_SIZE		(NUM_BITS_PER_LED * NUM_LEDS + RESET_SIGNAL_LENGTH)

// DMA descriptor has to be put in LPRAM to work
volatile DmacDescriptor ledDmaDesc __attribute__((aligned(16))) __attribute__ ((section(".lpram")));
DmacDescriptor writebackDesc __attribute__((aligned(16))) __attribute__ ((section(".lpram")));

static uint8_t ledSpiByteStream[LED_BYTESTREAM_SIZE];

void LedsInit()
{
	LedResetAllToDefault();
	LedUpdateColorDataAll();
}

void LedsDMAInit(sSPIDevice *device)
{
	Sercom *serialModule = device->sercomDevice;
	ledDmaDesc.BTCTRL.reg = DMAC_BTCTRL_VALID | DMAC_BTCTRL_SRCINC | DMAC_BTCTRL_BEATSIZE_BYTE;
	ledDmaDesc.BTCNT.reg = LED_BYTESTREAM_SIZE;
	// This has to point to the last element of the data
	ledDmaDesc.SRCADDR.reg = (uint32_t) (&ledSpiByteStream[LED_BYTESTREAM_SIZE]);
	ledDmaDesc.DSTADDR.reg = (uint32_t) &serialModule->SPI.DATA.reg;
	ledDmaDesc.DESCADDR.reg = 0;
	
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_DMAC;
	
	DMAC->CHID.reg = 0;
	DMAC->CTRL.bit.SWRST = 1;
	while(DMAC->CTRL.reg & DMAC_CTRL_SWRST);
	DMAC->CHCTRLA.bit.SWRST = 1;
	while(DMAC->CTRL.reg &DMAC_CTRL_SWRST);
	
	DMAC->CHID.reg = 0;
	DMAC->BASEADDR.reg = (uint32_t) &ledDmaDesc;
	DMAC->WRBADDR.reg = (uint32_t) &writebackDesc;
	DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_TRIGSRC(SERCOM1_DMAC_ID_TX) | DMAC_CHCTRLB_TRIGACT_BEAT;
	
	DMAC->CHID.reg = 0;
	DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xF);
	
	NVIC_EnableIRQ(DMAC_IRQn);
}

void LedUpdateColorDataAll()
{
	for(int i = 0; i < NUM_LEDS; i++)
	{
		LedUpdateColorData(i);
	}
	
	uint32_t resetIndex = NUM_LEDS * NUM_BITS_PER_LED;
	for(int i = 0; i < RESET_SIGNAL_LENGTH; i++)
	{
		ledSpiByteStream[resetIndex++] = 0;
	}
}

void LedUpdateColorData(uint8_t ledNum)
{
	uint32_t color;
	uint32_t ledDataIndex = ledNum * NUM_BITS_PER_LED;

	// This is intentional, data is sent as GRB on the Ws2812B LEDS used
	color = leds[ledNum].g << 16 | leds[ledNum].r << 8 | leds[ledNum].b;

	
	for(int bits = 23; bits >= 0; bits--)
	{
		if(((color >> bits) & 0x1) == 1)
		{
			ledSpiByteStream[ledDataIndex++] = 0b110;
		}
		else
		{
			ledSpiByteStream[ledDataIndex++] = 0b100;
		}
	}
}

void LedSendData(sSPIDevice *device)
{
	SPIWriteBuffer(device, ledSpiByteStream, LED_BYTESTREAM_SIZE);
}

void LedSendDataDMA()
{	
	DMAC->CHINTENCLR.reg = DMAC_CHINTENSET_TCMPL | DMAC_CHINTENSET_TERR;
	DMAC->CHCTRLA.bit.ENABLE = 1;
}


void LedSetColor(uint8_t ledNum, sRGBColor color)
{
	leds[ledNum] = color;
}

sRGBColor LedGetColor(uint8_t ledNum)
{
	return leds[ledNum];
}

void LedResetAllToDefault()
{
	for(int i = 0; i < NUM_LEDS; i++)
	{
		LedSetColor(i, DEFAULT_COLOR);
	}
	
	LedUpdateColorDataAll();
}

void LedDMACHandler()
{
	DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_MASK | DMAC_CHINTFLAG_TERR;
}