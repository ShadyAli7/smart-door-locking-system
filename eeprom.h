/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: external eeprom
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: header file for the external eeprom module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#ifndef EEPROM_H_
#define EEPROM_H_

#include"twi.h"
#include"micro_config.h"
#include"std_types.h"
#include"common_macros.h"
/***********************************************************************************************
 *                                  Preprocessor Macros                                        *
 ***********************************************************************************************/
#define ERROR 0
#define SUCCESS 1
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/***********************************************************************************************
 *                                  Functions Prototypes                                       *
 ***********************************************************************************************/
/*Description: function to initialize the External EEPROM  module
 * the function initializes the internal I2C module and takes one argument of type Twi_ConfigType
 * set the required Bit Rate
 * set the required slave address
 * set the required prescale
 */
void EEPROM_init(Twi_ConfigType *Twi_configPtr);

/* Description: function responsible for writing one byte in the External EEPROM
 * it takes two arguments: 1-the required address to store data , and 2-the required data to be stored
 * 1. send the start bit
 * 2. send the address of External EEPROM  with the MSB 3 Bits of internal address of EEPROM memory
 * 3. send the address of EEPROM in which the data will be stored at
 * 4. send the required data
 * 5. stop the I2C module
 * returns success or error
 */
uint8 EEPROM_writeByte(uint16 a_address,uint8 a_data);

/* Description: function responsible for reading one byte from the External EEPROM
 * it takes two arguments: 1-the required address the data stored at , and 2-pointer to the address of data
 * 1. send the start bit
 * 2. send the address of External EEPROM  with the MSB 3 Bits of internal address of EEPROM memory
 * 3. send the address of EEPROM in which the data will be stored at
 * 4. send repeated start
 * 5. send slave address
 * 6. read with negative ACK  and store the data at the pointer
 * 7. stop the I2C module
 * returns success or error
 */
uint8 EEPROM_readByte(uint16 a_address,uint8 *a_dataPtr);

/*Description: function responsible for writing 4 Bytes to the External EEPROM
 *it takes two arguments: 1-the required address to store data , and 2-the required data to be stored
 * send the four bytes by calling the function EEPROM_writeByte()
 */
void EEPROM_writeArray(uint16 a_address, uint8 *a_dataPtr);
/*Description: function responsible for reading 4 Bytes from the External EEPROM
 *it takes two arguments: 1-the required address of the data , and 2-the address in which the data will be stored
 * read the four bytes by calling the function EEPROM_readByte()
 */
void EEPROM_readArray(uint16 a_address , uint8 *a_dataPtr);

#endif /* EEPROM_H_ */
