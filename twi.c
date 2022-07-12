/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: twi
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: source file for AVR I2C module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#include"twi.h"
#include<stdio.h>

/****************************************************************************************************
 *                                       Functions Definitions                                      *
 ***************************************************************************************************/
/*
 * Description: function to initialize the I2C(TWI) module
 * it should take structure of type Twi_ConfigType holding the required information
 * set the required Bit Rate
 * set the required slave address
 * set the required prescale
 */
void TWI_init(Twi_ConfigType *Config_Ptr)
{
	/* calculating the value of register TWBR for the required Bit Rate and Prescale */
	TWBR=((F_CPU)/(2* Config_Ptr->s_bitRate *(4^Config_Ptr->prescale))) - (8/(4^Config_Ptr->prescale));
	/* setting the required Prescale in the two bits TWPS1,TWPS1 in register TWSR */
	TWSR= Config_Ptr->prescale;
	/* TWI bus requires slave address for each device if any master device want to call me */
	TWAR= Config_Ptr->s_slaveAddress;
	/* enable the TWI module  */
	TWCR=(1<<TWEN) ;
}
/*
 * Description: function responsible for sending the start condition of I2C module
 */
void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1 it is cleared by setting 1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 again
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) ;

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}
/*
 * Description: function responsible for sending the stop condition for I2C module
 */
void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
/*
 * Description: function responsible for sending one byte of data on the I2C bus
 * it takes one argument which the data to be transimetted
 */
void TWI_write(uint8 data)
{
    /* Put the required data to be transimetted On TWI data Register */
    TWDR = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) ;
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}
/*
 * Description: function responsible for reading one byte of data from the I2C bus and send
 * 				an ACK  after receiving
 * it returns one byte which is the received data
 */
uint8 TWI_readWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}
/*
 * Description: function responsible for reading one byte of data from the I2C bus and send
 * 				an Negative ACK  after receiving indicating that it is the last byte to be read
 * it returns one byte which is the received data
 */
uint8 TWI_readWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}
/*
 * Description: function responsible for getting the status of I2C
 * it returns one byte which is the value of TWSR register which is the status
 */
uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
