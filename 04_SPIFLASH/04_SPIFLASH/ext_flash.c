// Include EXT Flash Header File
#include "ext_flash.h"

// Supported Flash Devices
static const extFlashInfo_t flashInfo[] =
{
	{
		.manuID = MF_ADESTO,		// Adesto
		.devID = 0x01,				// Device ID
		.deviceSize = 0x100000		// 1Mbyte (8Mbit)
	},
	{
		.manuID = 0x0,
		.devID = 0x0,
		.deviceSize = 0x0
	}
};

// Flash Information
static const extFlashInfo_t *pFlashInfo = NULL;
static uint8_t infoBuf[2];
static uint8_t rxBuff[FLASH_PROGRAM_PAGE_SIZE + FLASH_MAX_COMMAND_SIZE];

/**
 * Callback fucntion for SPI transfer completed on EXTFLASH SPI.
 *
 * @param spi_m_async_descriptor		IO descriptor for SPI object.
 *
 * @return void
 */
static void SPI_EXTFLASH_complete_cb(const struct spi_m_async_descriptor *const io_descr)
{
	/* Transfer Completed */
}

/**
 * Method for initialising SPI on the EXTFLASH pins.
 *
 * @return void
 */
void EXTFLASH_init()
{
	// Get the IO Descriptor
	spi_m_async_get_io_descriptor(&SERIALFLASH, &extflash_io);
	
	// Register the Callback Function
	spi_m_async_register_callback(&SERIALFLASH, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)SPI_EXTFLASH_complete_cb);
	
	// Enable Asynchronous SPI
	spi_m_async_enable(&SERIALFLASH);
}

/**
 * Method for Transmitting to EXTFLASH via SPI.
 *
 * @param uint8_t *wbuf					Write buffer.
 * @param uint8_t *rbuf					Read buffer.
 * @param uint16_t length				Length of bytes to transfer.
 *
 * @return bool
 */
static bool EXTFLASH_transfer(const uint8_t *wbuf, uint8_t *rbuf, const uint16_t length)
{
	// Set the Initial Return Value
	bool returnVal = false;
	
	// Struct for the SPI Status
	struct spi_m_async_status p_stat;
	
	// Enable SPI
	gpio_set_pin_level(SERIALFLASH_CS, 0);
	
	// Perform ASYNC Transfer (send and receive)
	spi_m_async_transfer(&SERIALFLASH, &wbuf[0], &rbuf[0], length);
	
	// Wait until transfer completes...
	while (spi_m_async_get_status(&SERIALFLASH, &p_stat) == ERR_BUSY)
	{
		// Delay a bit before checking again
		delay_us(10);
	}
	
	// Set the Return Value
	returnVal = true;
	
	// Disable SPI
	gpio_set_pin_level(SERIALFLASH_CS, 1);
	
	// Return the Function
	return returnVal;
}

/**
 * Reads the flash information (manufacturer and device ID) from the device.
 *
 * @return bool
 */
static bool EXTFLASH_readInfo(void)
{
	// Create the Command Message
	const uint8_t wbuf[] = { FLASH_CMD_MDID, 0x00, 0x00, 0x00 };
	bool returnVal = false;
	
	// Send the Command
	if (EXTFLASH_transfer(wbuf, rxBuff, 4))
	{
		// Copy Vendor and Device Data into the Info Buffer
		infoBuf[0] = rxBuff[1];
		infoBuf[1] = rxBuff[3];
		
		// Return True
		returnVal = true;
	}
	
	// Return the Function
	return returnVal;
}

/**
 * Verify that the flash component is valid.
 *
 * @return bool
 */
static bool _extFlashVerifyPart(void)
{
	// Check that we can read the info in
	if (!EXTFLASH_readInfo())
	{
		return false;
	}
	
	// Point pFlashInfo to FlashInfo Array
	pFlashInfo = flashInfo;
	
	// Loop and Compare
	while(pFlashInfo->deviceSize > 0)
	{
		if (infoBuf[0] == pFlashInfo->manuID && infoBuf[1] == pFlashInfo->devID)
		{
			break;
		}
		pFlashInfo++;
	}
	
	// Return whether we found a matching device
	return (pFlashInfo->deviceSize > 0);
}

/**
 * Read the status register of the flash device.
 *
 * @param uint8_t *buf						Read buffer.
 *
 * @return bool
 */
static bool EXTFLASH_readStatus(uint8_t *buf)
{
	// Build the Command for Reading Flash Status
	const uint8_t wbuf[3] = { FLASH_CMD_READ_STATUS, 0x00, 0x00 };
	
	// Set Initial Return Value
	bool returnVal = false;
	
	// Send the Command get the Response
	if (EXTFLASH_transfer(wbuf, rxBuff, 3))
	{
		// Copy the Data into the Receive Buffer
		buf[0] = rxBuff[1];
		returnVal = true;
	}
	
	// Return the Function
	return returnVal;
}

/**
 * Wait for the previous erase / program operation to complete.
 *
 * @return int								0 on success, < 0 on failure
 */
static int EXTFLASH_waitReady(void)
{
	// Return Value
	int returnValue;
	
	for (;;)
	{
		uint8_t buf;
		returnValue = EXTFLASH_readStatus(&buf);
		
		// Check if Status Register Bit Busy
		if (!(buf & FLASH_STATUS_BIT_BUSY))
		{
			// Now Ready
			break;
		}
	}
	
	return 0;
}

/**
 * Enable writing on the flash device.
 *
 * @return int								0 on success, < 0 on failure
 */
static int EXTFLASH_writeEnable(void)
{
	// Command for Enabling Write Operations
	const uint8_t wbuf[] = { FLASH_CMD_WRITE_ENABLE };
	int returnValue = -3;
	
	// Send the Command over SPI
	if (EXTFLASH_transfer(wbuf, rxBuff, 1))
	{
		// Set our Return Value to Zero
		returnValue = 0;
	}
	
	// Return
	return returnValue;
}

/**
 * Configures the flash device for user.
 *
 * @return bool
 */
bool EXTFLASH_open(void)
{
	// Return Value (default to FALSE)
	bool returnValue = false;
	
	// Buffer for the Status Register
	uint8_t buf[1];
	
	// Verify the manufacturer and device ID
	if (_extFlashVerifyPart())
	{
		// Read the Status Register
		returnValue = EXTFLASH_readStatus(buf);
	}
	
	// Return
	return returnValue;
}

/**
 * Send a read request using 24-bit addressing.
 *
 * @param size_t offset		The byte offset in flash to begin reading from.
 * @param size_t length		The number of bytes to read.
 * @param uint8_t *buf		The buffer where data is stored. Must be at least {length} bytes in size.
 *
 * @return bool
 */
bool EXTFLASH_read(size_t offset, size_t length, uint8_t *buf)
{
	// Write Buffer for Command
	uint8_t wbuf[4];
	
	// Return Value (default to FALSE)
	bool returnValue = false;
	
	// Wait until previous command completes
	int ret = EXTFLASH_waitReady();
	
	if (ret)
	{
		return false;
	}
	
	// SPI is driven with low frequency (1Mhz), so not necessary to use fast read.
	wbuf[0] = FLASH_CMD_READ;
	wbuf[1] = (offset >> 16) & 0xFF;
	wbuf[2] = (offset >> 8) & 0xFF;
	wbuf[3] = offset & 0xFF;
	
	// Send the Read Command and Get the Response
	if (EXTFLASH_transfer(wbuf, rxBuff, length + 4))
	{
		// Copy the data over
		memcpy(&buf[0], &rxBuff[4], length);
		
		// Set the Return Value
		returnValue = true;
	}
	
	// Return the Function
	return returnValue;
}

/**
 * Send a write request using 24-bit addressing.
 *
 * @param size_t offset		The byte offset in flash to begin writing to.
 * @param size_t length		The number of bytes to write.
 * @param uint8_t *buf		The buffer where data to be written is stored. Must be at least {length} bytes in size.
 *
 * @return bool
 */
bool EXTFLASH_write(size_t offset, size_t length, const uint8_t *buf)
{
	// Write Buffer for Command
	uint8_t wbuf[4];
	
	// Status of SPI async
	struct spi_m_async_status p_stat;
	
	// Begin Write Loop
	while (length > 0)
	{
		// Wait until the previous operation is completed.
		int ret = EXTFLASH_waitReady();
		
		// If the device isn't ready, return false
		if (ret)
		{
			return false;
		}
		
		// Enable Writing on the Flash Device
		ret = EXTFLASH_writeEnable();
		
		// If the device isn't ready, return false
		if (ret)
		{
			return false;
		}
		
		// Interim length per instruction
		size_t ilen;
		
		// Work out the Instruction Length
		ilen = FLASH_PROGRAM_PAGE_SIZE - (offset % FLASH_PROGRAM_PAGE_SIZE);
		
		// Bound Checking
		if (length < ilen)
		{
			ilen = length;
		}
		
		// Create Command and 24 bit address
		wbuf[0] = FLASH_CMD_PROGRAM;
		wbuf[1] = (offset >> 16) & 0xFF;
		wbuf[2] = (offset >> 8) & 0xFF;
		wbuf[3] = offset & 0xFF;
		
		// Set Counters
		offset += ilen;
		length -= ilen;
		
		// Start the Combined Write Sequence
		gpio_set_pin_level(SERIALFLASH_CS, 0);
		
		// First, write the page program command
		if (spi_m_async_transfer(&SERIALFLASH, wbuf, rxBuff, 4) == ERR_NONE)
		{
			// Wait until transfer completes
			while (spi_m_async_get_status(&SERIALFLASH, &p_stat) == ERR_BUSY)
			{
				// Delay and Re-Check
				delay_us(10);
			}
		}
		else
		{
			// Failure
			gpio_set_pin_level(SERIALFLASH_CS, 1);
			
			// Return FALSE
			return false;
		}
		
		// Now write out the page of data
		if (spi_m_async_transfer(&SERIALFLASH, buf, rxBuff, ilen) == ERR_NONE)
		{
			// Wait until transfer completes
			while (spi_m_async_get_status(&SERIALFLASH, &p_stat) == ERR_BUSY)
			{
				// Delay and Re-Check
				delay_us(10);
			}
		}
		else
		{
			// Failure
			gpio_set_pin_level(SERIALFLASH_CS, 1);
			
			// Return FALSE
			return false;
		}
		
		// Complete the combined write sequence
		gpio_set_pin_level(SERIALFLASH_CS, 1);
		
		// Increment the Buffer
		buf += ilen;
	}
	
	// Return TRUE
	return true;
}

/**
 * Send an erase request using 24-bit addressing.
 *
 * @param size_t offset				The byte offset in flash to begin erasing from.
 * @param size_t length				The number of bytes to erase.
 *
 * @return bool
 */
bool EXTFLASH_erase(size_t offset, size_t length)
{
	// Write Buffer for Command
	uint8_t wbuf[4];
	size_t i, numsectors;
	
	// Set the Erase Command
	wbuf[0] = FLASH_CMD_SECTOR_ERASE;
	
	// Set the Offsets
	size_t endoffset = offset + length - 1;
	offset = (offset / FLASH_ERASE_SECTOR_SIZE) * FLASH_ERASE_SECTOR_SIZE;
	numsectors = (endoffset - offset + FLASH_ERASE_SECTOR_SIZE - 1) / FLASH_ERASE_SECTOR_SIZE;
	
	// Loop through each sector to erase
	for (i = 0; i < numsectors; i++)
	{
		// Wait until the previous operation is complete
		int ret = EXTFLASH_waitReady();
		
		// If not complete, return FALSE
		if (ret)
		{
			return false;
		}
		
		// Enable writing on the device
		ret = EXTFLASH_writeEnable();
		
		// If not complete, return FALSE
		if (ret)
		{
			return false;
		}
		
		// 24 bit sector address
		wbuf[1] = (offset >> 16) & 0xFF;
		wbuf[2] = (offset >> 8) & 0xFF;
		wbuf[3] = offset & 0xFF;
		
		// Send the Erase Command and Get the Response
		if (EXTFLASH_transfer(wbuf, rxBuff, 4)) {}
	}
	
	// Return TRUE
	return true;
}