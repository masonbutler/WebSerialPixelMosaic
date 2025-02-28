#include <sam.h>
#include "GPIO.h"

// Local function prototypes
static void ConfigMode(sGPIOPin *pin);
static void ConfigMux(sGPIOPin *pin);
static void ConfigPullUpDown(sGPIOPin *pin);


void GPIOPinInit(sGPIOPin *pin)
{
	MCLK->APBBMASK.reg |= MCLK_APBAMASK_PORT;
	ConfigMode(pin);
	ConfigPullUpDown(pin);
}

static void ConfigMode(sGPIOPin *pin)
{
	// Clear bits
	PORT->Group[pin->port].DIR.reg &= ~(0x1 << pin->pinNum);
	PORT->Group[pin->port].PINCFG[pin->pinNum].reg &= ~(PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN);

	if(pin->mode & GPIO_PIN_MODE_INPUT)
	{
		PORT->Group[pin->port].PINCFG[pin->pinNum].reg |= PORT_PINCFG_INEN;
	}
	else if(pin->mode & GPIO_PIN_MODE_OUTPUT)
	{
		PORT->Group[pin->port].DIR.reg |= (0x1 << pin->pinNum);
	}
	
	if(pin->mode & GPIO_PIN_MODE_MUXED)
	{
		PORT->Group[pin->port].PINCFG[pin->pinNum].reg |= PORT_PINCFG_PMUXEN;
		ConfigMux(pin);
	}
	
}

static void ConfigMux(sGPIOPin *pin)
{
	// MUX setting is divided between odd and even pin numbers
	if(pin->pinNum % 2 == 0)
	{
		PORT->Group[pin->port].PMUX[pin->pinNum >> 1].reg &= ~(PORT_PMUX_PMUXE_Msk);
		PORT->Group[pin->port].PMUX[pin->pinNum >> 1].reg |= PORT_PMUX_PMUXE(pin->muxSetting);
	}
	else
	{
		PORT->Group[pin->port].PMUX[pin->pinNum >> 1].reg &= ~(PORT_PMUX_PMUXO_Msk);
		PORT->Group[pin->port].PMUX[pin->pinNum >> 1].reg |= PORT_PMUX_PMUXO(pin->muxSetting);
	}
}



static void ConfigPullUpDown(sGPIOPin *pin)
{
	PORT->Group[pin->port].PINCFG[pin->pinNum].reg &= ~(PORT_PINCFG_PULLEN);
	// OUT reg is used to enable PULL up or PULL down when in output mode
	PORT->Group[pin->port].OUT.reg &= ~(0x1 << pin->pinNum);

	if(pin->mode & GPIO_PIN_MODE_INPUT)
	{
		if(pin->pullUpDown == GPIO_PULLUPDOWN_PULLDOWN)
		{
			PORT->Group[pin->port].PINCFG[pin->pinNum].reg |= PORT_PINCFG_PULLEN;
		}
		else if(pin->pullUpDown == GPIO_PULLUPDOWN_PULLUP)
		{
			PORT->Group[pin->port].PINCFG[pin->pinNum].reg |= PORT_PINCFG_PULLEN;
			// OUT reg is used to enable PULL up or PULL down when in output mode
			PORT->Group[pin->port].OUT.reg |= (0x1 << pin->pinNum);
		}
	}
}

inline void GPIOPinSetPin(sGPIOPin *pin)
{
	PORT->Group[pin->port].OUTSET.reg |= (0x1 << pin->pinNum);
}

inline void GPIOPinClearPin(sGPIOPin *pin)
{
	PORT->Group[pin->port].OUTCLR.reg |= (0x1 << pin->pinNum);
}

inline void GPIOPinTogglePin(sGPIOPin *pin)
{
	PORT->Group[pin->port].OUTTGL.reg |= (0x1 << pin->pinNum);
}

uint8_t GPIOPinReadPin(sGPIOPin *pin)
{
	uint32_t pinVal = PORT->Group[pin->port].IN.reg & (0x1 << pin->pinNum);
	return (0xFF & (pinVal >> pin->pinNum));
}

void GPIOPinSetMode(sGPIOPin *pin, uint8_t mode)
{
	pin->mode = mode;
	ConfigMode(pin);
}