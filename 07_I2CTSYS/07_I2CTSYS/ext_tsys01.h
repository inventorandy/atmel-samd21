/**
 * TSYS01 Digital Temperature Sensor Library
 *
 */
#ifndef EXT_TSYS01_H_
#define EXT_TSYS01_H_

// Include Drivers
#include "driver_init.h"

// Struct for I2C IO
struct io_descriptor *tsys_io;

// Define some commands
#define TSYS_CMD_RESET				0x1E
#define TSYS_CMD_START_ADC			0x48
#define TSYS_CMD_READ_ADC			0x00
#define TSYS_CMD_READ_PROM_0		0xA0
#define TSYS_CMD_READ_PROM_1		0xA2
#define TSYS_CMD_READ_PROM_2		0xA4
#define TSYS_CMD_READ_PROM_3		0xA6
#define TSYS_CMD_READ_PROM_4		0xA8
#define TSYS_CMD_READ_PROM_5		0xAA
#define TSYS_CMD_READ_PROM_6		0xAC
#define TSYS_CMD_READ_PROM_7		0xAE

// Coefficients for Temperature Conversion
#define COEFF_MUL_0					(float)(-1.5)
#define COEFF_MUL_1					(float)(1)
#define COEFF_MUL_2					(float)(-2)
#define COEFF_MUL_3					(float)(4)
#define COEFF_MUL_4					(float)(-2)

// Number of EEPROM Elements
#define PROM_ELEMS					8

// Temperature Sensor Methods
void init_tsys(void);
void read_coefficient(uint8_t cmd, uint16_t *coeff);
void read_eeprom(void);
void read_temperature(float *temperature);

#endif