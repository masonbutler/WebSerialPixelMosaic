#ifndef SPI_H_
#define SPI_H_
#include <sam.h>
#include "GPIO.h"

typedef enum {
	SPI_DATA_ORDER_MSB = 0,		/**< Send MSB first */
	SPI_DATA_ORDER_LSB,			/**< Send LSB first */
} SPI_DATA_ORDER;

typedef enum {
	SPI_CLOCK_POL_IDLE_LOW = 0,		/**< Clock idles on low on bus */
	SPI_CLOCK_POL_IDLE_HIGH,		/**< Clock idles on high on bus */
} SPI_CLOCK_POL;

typedef enum {
	SPI_CLOCK_PHA_SAMPLE_LEADING = 0,		/**< Samples data on leading edge */
	SPI_CLOCK_PHA_SAMPLE_TRAILING,			/**< Samples data on trailing edge */
} SPI_CLOCK_PHA;

typedef enum {
	SPI_DATAIN_PAD0 = 0,	/**< Data in (MISO for host, MOSI for client configuration) on PAD0 of sercom */
	SPI_DATAIN_PAD1,		/**< Data in (MISO for host, MOSI for client configuration) on PAD1 of sercom */
	SPI_DATAIN_PAD2,		/**< Data in (MISO for host, MOSI for client configuration) on PAD2 of sercom */
	SPI_DATAIN_PAD3			/**< Data in (MISO for host, MOSI for client configuration) on PAD3 of sercom */
} SPI_DATAIN_PAD;

typedef enum {
	SPI_DATAOUT_PAD_CONFIG0 = 0,	/**< DATAOUT- PAD0, SCK - PAD1, SS (client) - PAD2 */
	SPI_DATAOUT_PAD_CONFIG1,		/**< DATAOUT- PAD2, SCK - PAD3, SS (client) - PAD1 */
	SPI_DATAOUT_PAD_CONFIG2,		/**< DATAOUT- PAD3, SCK - PAD1, SS (client) - PAD2 */
	SPI_DATAOUT_PAD_CONFIG3			/**< DATAOUT- PAD0, SCK- PAD3, SS (client) - PAD1 */
} SPI_DATAOUT_PAD;

typedef enum {
	SPI_OPERATING_MODE_CLIENT = 2,	/**< MCU acts as client device, other device is the master */	
	SPI_OPERATING_MODE_HOST = 3,	/**< MCU acts as master device */
} SPI_OPERATING_MODE;

typedef enum {
	SPI_DUPLEX_TXONLY = 0,			/**< Half duplex, only transmits */
	SPI_DUPLEX_FULL,				/**< Full duplex, transmits and receives */
} SPI_DUPLEX_MODE;

typedef enum {
	SPI_SS_CONTROL_SOFTWARE = 0,	/**< Slave select controlled manually in software*/
	SPI_SS_CONTROL_HARDWARE,		/**< Slave select controlled by hardware */
} SPI_SS_CONTROL;

typedef struct {
	Sercom *sercomDevice;			/**< Pointer to the sercom peripheral */
	uint8_t sercomNumber;			/**< The number of the sercom module (sercom1, sercom2, etc) */
	sGPIOPin *softwareSSPin;		/**< Pointer to the GPIO pin used as SS (slave select) */
	uint32_t clockSpeed;			/**< Clock speed */
	uint8_t operatingMode : 2;		/**< Operating mode (client or host) */
	uint8_t dataOrder : 1;			/**< Whether the device sends MSB or LSB first */
	uint8_t clockPol : 1;			/**< Clock idle polarity */
	uint8_t clockPha : 1;			/**< Clock sample on leading or trailing edge */
	uint8_t dataInPad : 2;			/**< DATAIN pad number */
	uint8_t dataOutPadConfig : 2;	/**< DATAOUT pad configuration */
	uint8_t duplexMode : 1;			/**< Half-duplex (TX only) or full-duplex */
} sSPIDevice;


/*!
* Initialize the SPI peripheral
* @param[in] device The SPIDevice configuration struct
*/
void SPIInit(sSPIDevice *device);
/*!
* Enable the SPI peripheral
* @param[in] device The SPIDevice configuration struct
*/
void SPIEnable(sSPIDevice *device);
/*!
* Disable the SPI peripheral
* @param[in] device The SPIDevice configuration struct
*/
void SPIDisable(sSPIDevice *device);
/*!
* Set the SS pin to high
* @param[in] device The SPIDevice configuration struct
*/
void SPISetSS(sSPIDevice *device);
/*!
* Clear the SS pin to low
* @param[in] device The SPIDevice configuration struct
*/
void SPIClearSS(sSPIDevice *device);
/*!
* Write the buffer to the SPI periph
* @param[in] device The SPIDevice configuration struct
* @param[in] buffer A pointer to the data buffer
* @param[in] size The size of the data to write to the SPI periph
*/
void SPIWriteBuffer(sSPIDevice *device, uint8_t *buffer, uint32_t size);
/*!
* Read a byte from the SPI periph
* @param[in] device The SPIDevice configuration struct
* @return Returns the byte read from the SPI periph
*/
uint8_t SPIReadByte(sSPIDevice *device);

#endif