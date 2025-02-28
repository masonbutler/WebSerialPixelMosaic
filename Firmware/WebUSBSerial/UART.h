#ifndef UART_H_
#define UART_H_
#include <sam.h>

typedef enum {
	UART_RX_PAD0 = 0,		/**< RX pin on PAD0 */
	UART_RX_PAD1,			/**< RX pin on PAD1 */
	UART_RX_PAD2,			/**< RX pin on PAD2 */
	UART_RX_PAD3			/**< RX pin on PAD3 */
} UART_RX_PAD;

typedef enum {
	UART_TX_PAD_CONFIG0 = 0,	/**< TX -> PAD 0, XCK PAD 1, RTS N/A CTS N/A */
	UART_TX_PAD_CONFIG1,		/**< TX -> PAD 2, XCK Pad3, RTS N/A CTS N/a */
	UART_TX_PAD_CONFIG2,		/**< TX -> PAd0,  rts pad 2, cts pad 3 */
	UART_TX_PAD_CONFIG3,		/**< TX-> pad 0, xck pad 1,  rts pad 2, cts n/a*/
} UART_TX_PAD;

typedef enum {
	UART_PARITY_EVEN = 0,		/**< Even parity */
	UART_PARITY_ODD,			/**< Odd parity */
} UART_PARITY;

typedef enum {	
	UART_STOPBITS_NUM_1 = 0,	/**< 1 Stop bit */
	UART_STOPBITS_NUM_2,		/**< 2 Stop bits */
} UART_STOPBITS_NUM;

typedef enum {
	UART_CHARSIZE_8BITS = 0,	/**< 8-bit character size  */
	UART_CHARSIZE_9BITS,		/**< 9-bit character size  */
	UART_CHARSIZE_5BITS = 5,	/**< 5-bit character size  */
	UART_CHARSIZE_6BITS,		/**< 6-bit character size  */
	UART_CHARSIZE_7BITS,		/**< 7-bit character size  */
} UART_CHARSIZE;

typedef enum {
	UART_TRANSCEIVER_ENABLE_NONE = 0,	/**< Disable TX and RX  */
	UART_TRANSCEIVER_ENABLE_TX = 1,		/**< Enable TX and disable RX */
	UART_TRANSCEIVER_ENABLE_RX,			/**< Disable TX and enable RX */
} UART_TRANSCEIVER_ENABLE;

typedef struct  
{
	Sercom *sercomDevice;					/**< Pointer to the sercom peripheral */
	uint8_t rxPadPos : 3;					/**< RX pad position number */
	uint8_t txPadPos : 3;					/**< TX pad position number*/
	uint8_t parity : 1;						/**< Even or odd parity */
	uint8_t numStopBits : 1;				/**< Number of stop bits */
	uint8_t charSize : 3;					/**< Number of bits in each character */
	uint32_t baud;							/**< Baud rate */
	uint8_t transceiverEnable: 2;			/**< Enable the TX, RX, or both */
} sUARTDevice;

/*!
* Initialize the UART peripheral
* @param[in] device The UARTDevice configuration struct
* @param[in] sercomNumber The number of the sercom module
*/
void UARTInit(sUARTDevice *device, uint8_t sercomNumber);
/*!
* Enable the UART peripheral
* @param[in] device The UARTDevice configuration struct
*/
void UARTEnable(sUARTDevice *device);
/*!
* Disable the UART peripheral
* @param[in] device The UARTDevice configuration struct
*/
void UARTDisable(sUARTDevice *device);
/*!
* Write the string to the UART periph
* @param[in] device The UARTDevice configuration struct
* @param[in] string A pointer to the string to transmit
*/
void UARTWrite(sUARTDevice *device, char *string);
/*!
* Read a byte from the UART periph
* @param[in] device The UARTDevice configuration struct
* @return Returns the byte read from the UART periph
*/
uint8_t UARTRead(sUARTDevice *device);


#endif