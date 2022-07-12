/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: external eeprom
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: source file for the external eeprom module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#include "eeprom.h"
/**************************************************************************************************
 *                                     Functions Definitions                                      *
 **************************************************************************************************/
/*Description: function to initialize the External EEPROM  module
 * the function initializes the internal I2C module and takes one argument of type Twi_ConfigType
 * set the required Bit Rate
 * set the required slave address
 * set the required prescale
 */
void EEPROM_init(Twi_ConfigType *Twi_configPtr)
{	/* initialize the internal I2C module with the required information through
     *a structure of type Twi_ConfigType */
	void TWI_init(Twi_ConfigPtr);
}

/* Description: function responsible for writing one byte in the External EEPROM
 * it takes two arguments: 1-the required address to store data , and 2-the required data to be stored
 * 1. send the start bit
 * 2. send the address of External EEPROM  with the MSB 3 Bits of internal address of EEPROM memory
 * 3. send the address of EEPROM in which the data will be stored at
 * 4. send the required data
 * 5. stop the I2C module
 * returns success or error
 */
uint8 EEPROM_writeByte(uint16 a_address,uint8 a_data)
{
	/* Send the Start Bit */
    TWI_start();
    /* check if the start condition has been sent successfully */
    if (TWI_getStatus() != TW_START)
        return ERROR;
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)(0xA0 | ((a_address & 0x0700)>>7)));
    /* check if the master transmit (slave address + write request) has been sent successfully
     * and received ACK */
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;
    /* Send the required memory location address */
    TWI_write((uint8)(a_address));
    /* check if the master transmit address of EEPROM in which the data will be stored at
     *  has been sent successfully and received ACK */
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
    /* write byte to eeprom */
    TWI_write(a_data);
    /* check if the master transmit data has been sent successfully
     * and received ACK */
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
    /* Send the Stop Bit */
    TWI_stop();
    return SUCCESS;  /* return success if the byte has been sent successfullyu */
}

/* Description: function responsible for reading one byte from the External EEPROM
 * it takes two arguments:1- the required address the data stored at , and 2- pointer to the address of data
 * 1. send the start bit
 * 2. send the address of External EEPROM  with the MSB 3 Bits of internal address of EEPROM memory
 * 3. send the address of EEPROM in which the data will be stored at
 * 4. send repeated start
 * 5. send slave address
 * 6. read with negative ACK  and store the data at the pointer
 * 7. stop the I2C module
 * returns success or error
 */
uint8 EEPROM_readByte(uint16 a_address, uint8 *a_dataPtr)
{
	/* Send the Start Bit */
    TWI_start();
    /* check if the start condition has been sent successfully */
    if (TWI_getStatus() != TW_START)
        return ERROR;
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)((0xA0) | ((a_address & 0x0700)>>7)));
    /* check if the master transmit (slave address + write request) has been sent successfully
     * and received ACK */
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;
    /* Send the required memory location address */
    TWI_write((uint8)(a_address));
    /* check if the master transmit address of EEPROM in which the data will be stored at
     *  has been sent successfully and received ACK */
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
    /* Send the Repeated Start Bit */
    TWI_start();
    /* check if the repeated start bit has been sent successfully */
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_write((uint8)((0xA0) | ((a_address & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;
    /* Read Byte from Memory without send ACK */
    *a_dataPtr = TWI_readWithNACK();
    /* check if the master received the data with negative ACK */
    if (TWI_getStatus() != TW_MR_DATA_NACK)
        return ERROR;
    /* Send the Stop Bit */
    TWI_stop();
    return SUCCESS; /* chack if the byte has been received successfully */
}

/*Description: function responsible for writing 4 Bytes to the External EEPROM
 *it takes two arguments: 1-the required address to store data , and 2-the required data to be stored
 * send the four bytes by calling the function EEPROM_writeByte()
 */
void EEPROM_writeArray(uint16 a_address, uint8 *a_dataPtr)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		EEPROM_writeByte((a_address + i) , *a_dataPtr);
		a_dataPtr++;
		_delay_ms(10);
	}
}

/*Description: function responsible for reading 4 Bytes from the External EEPROM
 *it takes two arguments: 1-the required address of the data , and 2-the address in which the data will be stored
 * read the four bytes by calling the function EEPROM_readByte()
 */
void EEPROM_readArray(uint16 a_address , uint8 *a_dataPtr)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		EEPROM_readByte((a_address + i) , a_dataPtr);
		a_dataPtr++;
		_delay_ms(10);
	}
}
