/**
 * TSYS01 Digital Temperature Sensor Library/
 *
 */
#include "ext_tsys01.h"

// Flag for Read Coefficients
static bool coeff_read = false;

// Coefficient Multipliers
static const float coeff_mul[5] = { COEFF_MUL_0, COEFF_MUL_1, COEFF_MUL_2, COEFF_MUL_3, COEFF_MUL_4 };

static uint16_t eeprom_coeff[PROM_ELEMS];

static uint32_t adc = 0;

/**
 * Initialise I2C on the board and set the slave address.
 *
 */
void init_tsys()
{
	// Initialise the I2C I/O Communications
	i2c_m_sync_get_io_descriptor(&TSYS, &tsys_io);
	i2c_m_sync_enable(&TSYS);
	i2c_m_sync_set_slaveaddr(&TSYS, 0x77, I2C_M_SEVEN);
	
	// Briefly Reset the Device
	uint8_t icmd[1] = { TSYS_CMD_RESET };
	io_write(tsys_io, icmd, 1);
	delay_ms(10);
}

/**
 * Read a single coefficient specified by (cmd), and save to 16 bit unsigned (*coeff)
 *
 */
void read_coefficient(uint8_t cmd, uint16_t *coeff)
{
	// Read Buffer
	uint8_t buffer[2] = { 0x00, 0x00 };
	
	// Write Command
	uint8_t icmd[1] = { cmd };
	
	// Write the Command
	io_write(tsys_io, icmd, 1);
	
	// Short Delay
	delay_ms(20);
	
	// Read Two Bytes
	io_read(tsys_io, &buffer[0], 2);
	
	// Convert to 16 bit unsigned int
	*coeff = (buffer[0] << 8) | buffer[1];
}

/**
 * Read the EEPROM Values for Temperature Conversion
 *
 */
void read_eeprom()
{
	// Counter in the Loop
	uint8_t i;
	
	// Loop through the Coefficients
	for (i = 0; i < PROM_ELEMS; i++)
	{
		// Read the Coefficient
		read_coefficient(TSYS_CMD_READ_PROM_0 + i * 2, eeprom_coeff + i);
	}
	
	// Set the Coefficient Flag
	coeff_read = true;
}

void read_temperature(float *temperature)
{
	// Float for Temperature
	float temp = 0;
	
	// Command Array
	uint8_t icmd[1] = { 0x00 };
	
	// Check if we've read the Coefficients
	if (coeff_read == false)
	{
		// Read the Coefficients
		read_eeprom();
	}
	
	// Send the Start ADC Command
	icmd[0] = TSYS_CMD_START_ADC;
	io_write(tsys_io, icmd, 1);
	
	// Short Delay
	delay_ms(20);
	
	// Send the Read ADC Command
	icmd[0] = TSYS_CMD_READ_ADC;
	io_write(tsys_io, icmd, 1);
	
	// Short Delay
	delay_ms(20);
	
	// Buffer for the Result
	uint8_t buffer[3] = { 0x00, 0x00, 0x00 };
	
	// Read the Result
	io_read(tsys_io, &buffer[0], 3);
	
	// 32 bit unsigned for ADC Result
	adc = ((buffer[0] << 16) | (buffer[1] << 8) | buffer[2]);
	
	delay_us(10);
	
	// Divide by 256
	adc /= 256;
	
	// Counter in For Loop
	uint8_t i;
	
	// Loop through and Multiply
	for (i = 4; i > 0; i--)
	{
		// Multiply by Coefficient
		temp += coeff_mul[i] * eeprom_coeff[1 + (4 - i)];
		temp *= (float)adc / 100000;
	}
	
	// Multiply by 10
	temp *= 10;
	
	// Add the Next Coefficient
	temp += coeff_mul[0] * eeprom_coeff[5];
	
	// Divide by 100
	temp /= 100;
	
	// Set the Temperature
	*temperature = temp;
	
	// Return True
	//return true;
}