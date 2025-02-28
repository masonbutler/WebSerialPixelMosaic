#ifndef GPIO_H_
#define GPIO_H_
#include <sam.h>

typedef enum {
	GPIO_PORT_A = 0,	/**< Port A */
	GPIO_PORT_B,		/**< Port B */
} GPIO_PORT_NAME;


typedef enum {
	GPIO_PIN_MODE_INPUT = 1,		/**< Input mode */
	GPIO_PIN_MODE_OUTPUT = 2,		/**< Output mode */
	GPIO_PIN_MODE_MUXED = 4,		/**< Mux mode */
} GPIO_PIN_MODE;

typedef enum {
	GPIO_MUX_SETTING_NONE = 0,
	GPIO_MUX_SETTING_A = 0,
	GPIO_MUX_SETTING_B,
	GPIO_MUX_SETTING_C,
	GPIO_MUX_SETTING_D,
	GPIO_MUX_SETTING_E,
	GPIO_MUX_SETTING_F,
	GPIO_MUX_SETTING_G,
	GPIO_MUX_SETTING_H,
	GPIO_MUX_SETTING_I,
} GPIO_MUX_SETTING;

// Only available when pin set to INPUT
typedef enum {
	GPIO_PULLUPDOWN_NONE = 0,		/**< No pull-up or pull-down active */
	GPIO_PULLUPDOWN_PULLUP = 1,		/**< Pull-up active */
	GPIO_PULLUPDOWN_PULLDOWN = 2,	/**< Pull-down active */
} GPIO_PULLUPDOWN ;

/**
* A structure of a GPIO pin, its configuration settings
*/
typedef struct {
	uint8_t port : 1;					/**< Letter of the port the pin is on */
	uint8_t pinNum : 5;					/**< Pin number on the port, i.e PB9, number is 9, 0-31 */
	uint8_t mode : 3;					/**< Pin output, input, or alternate function */
	uint8_t pullUpDown: 2;				/**< Pin pull-up or pull-down configuration */
	uint8_t muxSetting: 4;				/**< The alternate function number when MUX is set (see MCU datasheet) */
} sGPIOPin;

/*!
* Initialize a GPIO pin
* @param[in] pin The pointer to the GPIOPin struct defining the pin
* @return The result of the action (success or error)
*/
void GPIOPinInit(sGPIOPin *pin);

/*!
* Set a pin to HIGH (logic 1)
* @param[in] pin The pointer to the GPIOPin struct defining the pin
*/
void GPIOPinSetPin(sGPIOPin *pin);

/*!
* Clear a pin to LOW (logic 0)
* @param[in] pin The pointer to the GPIOPin struct defining the pin
*/
void GPIOPinClearPin(sGPIOPin *pin);

/*!
* Toggle the value of a pin
* @param[in] pin The pointer to the GPIOPin struct defining the pin
*/
void GPIOPinTogglePin(sGPIOPin *pin);

/*!
* Read the value of a pin and return it
* @param[in] pin The pointer to the GPIOPin struct defining the pin
*/
uint8_t GPIOPinReadPin(sGPIOPin *pin);

/*!
* Set the pin mode
* @param[in] pin The pointer to the GPIOPin struct defining the pin
* @param[in] the mode to set the pin to
*/
void GPIOPinSetMode(sGPIOPin *pin, uint8_t mode);

#endif 