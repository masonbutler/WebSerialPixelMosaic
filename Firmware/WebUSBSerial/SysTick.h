#ifndef SYSTICK_H_
#define SYSTICK_H_
#include <stdint.h>

void SysTickInit(uint32_t freq);
void SysTickStart();
void SysTickStop();
void SysTickHandler();
uint32_t SysTickGetNumTicks();


#endif