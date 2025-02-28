#include "sam.h"
#include "System.h"
#include "SysTick.h"
#include "WebUsbSerialApp.h"

int main(void)
{
    SystemClock48MhzInit();
	__enable_irq();
	SysTickInit(5); // Tick to show device hasn't frozen
	SysTickStart();

	WebSerialAppInit();

    while (1) 
    {
		
	}
}