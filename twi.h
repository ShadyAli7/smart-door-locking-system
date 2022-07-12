/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: twi
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: header file for AVR I2C module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#ifndef TWI_H_
#define TWI_H_

#include"common_macros.h"
#include"std_types.h"
#include"micro_config.h"

/******************************************************************************************
 *                                 Preprocessor Macros                                    *
 ******************************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/***************************************************************************************
 *                                 Types Declaration                                   *
 **************************************************************************************/
/*
 * Description: enumeration to select Clock prescale for the I2C module
 */
typedef enum
{
	F_CPU_1,F_CPU_4,F_CPU_16,F_CPU3_64
}Twi_Prescaler;
/* Description : structure to store the required information to initialize the I2C module
 * the structure have 3 members
 * 1. the number of required Bit Rate
 * 2. set the required Prescale
 * 3. set the required slave address for the device
 */
typedef struct
{
	uint32 s_bitRate;
	Twi_Prescaler prescale;
	uint8 s_slaveAddress;
}Twi_ConfigType;

/******************************************************************************************
 *                                 Functions Prototypes                                   *
 ******************************************************************************************/
/*
 * Description: function to initialize the I2C(TWI) module
 * it should take structure of type Twi_ConfigType holding the required information
 * set the required Bit Rate
 * set the required slave address
 * set the required prescale
 */
void TWI_init(Twi_ConfigType *Config_Ptr);
/*
 * Description: function responsible for sending the start condition of I2C module
 */
void TWI_start(void);
/*
 * Description: function responsible for sending the stop condition for I2C module
 */
void TWI_stop(void);
/*
 * Description: function responsible for sending one byte of data on the I2C bus
 * it takes one argument which the data to be transimetted
 */
void TWI_write(uint8 data);
/*
 * Description: function responsible for reading one byte of data from the I2C bus and send
 * 				an ACK  after receiving
 * it returns one byte which is the received data
 */
uint8 TWI_readWithACK(void); //read with send Ack
/*
 * Description: function responsible for reading one byte of data from the I2C bus and send
 * 				an Negative ACK  after receiving indicating that it is the last byte to be read
 * it returns one byte which is the received data
 */
uint8 TWI_readWithNACK(void); //read without send Ack
/*
 * Description: function responsible for getting the status of I2C
 * it returns one byte which is the value of TWSR register which is the status
 */
uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
