#ifndef LEDS_H_
#define LEDS_H_
#include "SPI.h"

#define NUM_LEDS	64U

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} sRGBColor;

#define COLOR_OFF		((sRGBColor) {0, 0, 0})
#define DEFAULT_COLOR	COLOR_OFF

sRGBColor leds[NUM_LEDS];

void LedsInit();
void LedsDMAInit(sSPIDevice *device);
void LedUpdateColorData(uint8_t ledNum);
void LedUpdateColorDataAll();
void LedSendData(sSPIDevice *device);
void LedSendDataDMA();

void LedSetColor(uint8_t ledNum, sRGBColor color);
void LedResetAllToDefault();
sRGBColor LedGetColor(uint8_t ledNum);

void LedDMACHandler();

#endif