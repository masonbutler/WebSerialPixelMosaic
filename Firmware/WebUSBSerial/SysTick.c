#include "SysTick.h"
#include "sam.h"
#include "System.h"

static uint32_t ticks = 0;

void SysTickInit(uint32_t freq)
{
	// We get our desired interrupt frequency by dividing clock by our freq and SysTick is 24-bits so we mask those bits
	uint32_t period = (SYS_CLK / freq) & 0xFFFFFF;
	SysTick->VAL = 0;
	SysTick->LOAD = period;
	ticks = 0;
	// Init sanity LED
	PORT->Group[1].DIR.reg |= (0x1 << 10);
	PORT->Group[1].OUTCLR.reg |= (0x1 << 10);
	
	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTickStart()
{
	// Enable SysTick and enable tick interrupt
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

void SysTickStop()
{
	// Disable SysTick and tick interrupt
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

void SysTickHandler()
{
	// Increase number of ticks
	ticks++;
	// Blink sanity LED

	PORT->Group[1].OUTTGL.reg |= (0x1 << 10);
}

uint32_t SysTickGetNumTicks()
{
	return ticks;
}