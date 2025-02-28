#include "sam.h"
#include "System.h"
#include "NVM_OTP.h"

static void ConfigurePowerManager();
static void ConfigurePLL();
void SystemClock48MhzInit()
{
	// Calibration value from NVM
	
	ConfigurePowerManager();

	// Set main clock dividers
	MCLK->LPDIV.reg = MCLK_LPDIV_LPDIV_DIV4;
	MCLK->BUPDIV.reg = MCLK_LPDIV_LPDIV_DIV8;
	// Wait for clock to be ready
	while(!(MCLK->INTFLAG.reg & MCLK_INTFLAG_CKRDY));
	// Enable internal OSC and set it to 16 MHz
	OSCCTRL->OSC16MCTRL.reg = OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_FSEL_16;
	
	// Set wait states for NVM according to osc freq
	NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS(2);
	// Enable brown out interrupts
	//SUPC->INTFLAG.reg = SUPC_INTFLAG_BOD33RDY | SUPC_INTFLAG_BOD33DET;
	
	ConfigurePLL();
	
}

static void ConfigurePowerManager()
{
	PM->INTFLAG.reg = PM_INTFLAG_PLRDY;
	PM->PLCFG.reg = PM_PLCFG_PLSEL_PL2;
	while (!PM->INTFLAG.reg);
}

static void ConfigurePLL()
{
	uint32_t coarse = *((uint32_t *)(NVMCTRL_OTP5)) >> NVM_OTP_DFLL48M_COARSE_CAL_POS;

	
	OSCCTRL->INTFLAG.reg = OSCCTRL_INTFLAG_DFLLRDY | OSCCTRL_INTFLAG_OSC16MRDY;

	// This is a workaround to a HW bug found in errata # 1.3.1
	// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/Errata/SAM-L21-Family-Silicon-Errata-and-Data-Sheet-Clarification-DS80000781.pdf
	OSCCTRL->DFLLCTRL.reg = 0;
	// Wait for DFLL to be ready
	while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));
	
	// Configure the DFLL multiplier for desired freq
	OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_MUL(48000) | OSCCTRL_DFLLMUL_CSTEP(1) | OSCCTRL_DFLLMUL_FSTEP(1);
	// Wait for DFLL to be ready
	while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));
	// Set calibration values read from NVM
	OSCCTRL->DFLLVAL.reg = OSCCTRL_DFLLVAL_COARSE(coarse) | OSCCTRL_DFLLVAL_FINE(0x200);
	// Wait for DFLL to be ready
	while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));
	// Enable DFLL and set to closed-loop and set it into USB clock recovery mode to stabilize USB clock using SOF sequences
	OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_ENABLE | OSCCTRL_DFLLCTRL_MODE | OSCCTRL_DFLLCTRL_USBCRM;
	// Wait for DFLL to be ready
	while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));

	// Set DFLL as main clock source
	GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_SOURCE_DFLL48M) | GCLK_GENCTRL_GENEN;
	// Wait for main clock to sync
	while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);
}