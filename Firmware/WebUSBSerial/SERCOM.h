#ifndef SERCOM_H_
#define SERCOM_H_
#include <sam.h>

#define NUM_SERCOM_MODULES	6U

extern Sercom *sercomAddr[];
extern uint32_t sercomClockId[];
extern uint32_t sercomAPBCMask[];
extern uint32_t sercomAPBDMask[];

typedef enum {
	SERCOM_PERIPH_NUM_0 = 0,  /**< Sercom peripheral 0 */
	SERCOM_PERIPH_NUM_1,	  /**< Sercom peripheral 1 */
	SERCOM_PERIPH_NUM_2,	  /**< Sercom peripheral 2 */
	SERCOM_PERIPH_NUM_3,	  /**< Sercom peripheral 3 */
	SERCOM_PERIPH_NUM_4,	  /**< Sercom peripheral 4 */
	SERCOM_PERIPH_NUM_5		  /**< Sercom peripheral 5 */
} SERCOM_PERIPH_NUM;

/*!
* Enable master clock and clock mask for the sercom module
* @param[in] sercomNumber The number of the sercom module to enable
*/
void SercomClockEnable(uint8_t sercomNumber);


#endif