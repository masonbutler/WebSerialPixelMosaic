#include "SERCOM.h"

 Sercom *sercomAddr[] = {
	 SERCOM0,
	 SERCOM1,
	 SERCOM2,
	 SERCOM3,
	 SERCOM4,
	 SERCOM5,
 };
 
 uint32_t sercomClockId[] = {
	 SERCOM0_GCLK_ID_CORE,
	 SERCOM1_GCLK_ID_CORE,
	 SERCOM2_GCLK_ID_CORE,
	 SERCOM3_GCLK_ID_CORE,
	 SERCOM4_GCLK_ID_CORE,
	 SERCOM5_GCLK_ID_CORE,
 };
 
uint32_t sercomAPBCMask[] = {
	MCLK_APBCMASK_SERCOM0,
	MCLK_APBCMASK_SERCOM1,
	MCLK_APBCMASK_SERCOM2,
	MCLK_APBCMASK_SERCOM3,
	MCLK_APBCMASK_SERCOM4,
};

uint32_t sercomAPBDMask[] = {
	MCLK_APBDMASK_SERCOM5,
};

void SercomClockEnable(uint8_t sercomNumber)
{
	if(sercomNumber < 5)
	{
		MCLK->APBCMASK.reg |= sercomAPBCMask[sercomNumber];
	}
	else if(sercomNumber == 5)
	{
		// Sercom5 is on APBD so to index into the array, we have to subtract the number of sercoms - 1
		// to get index 0 in the array from the sercom number  (6 - 1 = 5) for example
		MCLK->APBDMASK.reg |= sercomAPBDMask[sercomNumber - (NUM_SERCOM_MODULES - 1)];
	}

	GCLK->PCHCTRL[sercomClockId[sercomNumber]].reg = (GCLK_PCHCTRL_GEN(0) | GCLK_PCHCTRL_CHEN);
	while(!(GCLK->PCHCTRL[sercomClockId[sercomNumber]].reg & GCLK_PCHCTRL_CHEN));
}