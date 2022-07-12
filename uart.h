/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: timer.h
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: header file for the UART module in  AVR Micro controller
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#ifndef UART_H_
#define UART_H_

#include"common_macros.h"
#include"std_types.h"
#include"micro_config.h"


/***************************************************************************************
 *                                 Types Declaration                                   *
 **************************************************************************************/
/*
 * Description: enumeration to select the mode of the receiver of UART module
 */
typedef enum
{
	polling , interrupt
}Uart_Mode;

/*
 * Description: enumeration to select the parity bit of the UART frame
 */
typedef enum
{
	disable , reserved , even , odd
}Uart_ParityBit;

/*
 * Description: enumeration to select the number of stop bits for the UART frame
 */
typedef enum
{
	one_bit , two_bit
}Uart_StopBit;

/*
 * Description: enumeration to select the number of data bits will be transmitted or received
 */
typedef enum
{
	five_bit , six_bit , seven_bit , eight_bit , reserved1 , reserved2 , reserved3 , nine_bit
}Uart_DataBit;

/* Description : structure to store the required information to initialize the UART module
 * the structure have 5 members
 * 1. value of baud rate the UART will use to synchronize
 * 2. Mode of the Receiver if its : Interrupt or Polling
 * 3. number of data bits will be used
 * 4. the required parity or to disable it
 * 5.number of stop bits for the UART module
 */
typedef struct
{
	uint32 s_baudRate;     /* value of the required baud rate */
	Uart_Mode mode;        /* required mode for the receiver */
	Uart_DataBit data;     /* number of data bits transimetted or received */
	Uart_ParityBit parity; /* the required parity */
	Uart_StopBit stop;	   /* number of stop bits for the frame */
}Uart_ConfigType;

/**************************************************************************************
 *                             Functions Prototypes                                   *
 **************************************************************************************/

/*
 * Description : Function to initialize the UART Driver
 * 	1.set the value of baud rate
 * 	2.set the required mode for the Receiver of UART
 * 	3.Set the required number of Data bits transmitted or Received
 * 	4.set the required parity bit or disable it
 * 	5.set the required number of stop bits
 */
/* the function takes one argument a structure of type : Uart_ConfigType */
void UART_init(const Uart_ConfigType *Config_Ptr);

/* Description: function to send one byte of data using UART module
 * the function should take one argument only which is the required data to be transmitted
 * the argument type is : uint16
 */
void UART_sendByte(uint16 a_data);

/* Description: function to receive one byte of data using UART module
 * the function do not take arguments
 * the function should return the received data by UART module
 */
uint16 UART_receiveByte(void);

/* Description: function to send string using UART module
 * the function should take one argument only which is the required string to be transmitted
 * the function should send the string byte by byte using the function UART_sendByte
 */
void UART_sendString(const uint8 *str);

/* Description: function to receive string using UART module
 * the function takes one argument: the address the string will be stored at
 * the function receives the string byte by byte using the function UART_receiveByte
 */
void UART_receiveString(uint8 *str);

/*[Description]: function to send an array using UART module
 * [Arguments]: it should take two arguments:
 * 1. address of the array to be sent
 * 2. the size of the array
 * [returns]: the function returns void
 */

void UART_sendArray(uint8 *a_dataPtr , uint8 a_arraySize);

/*[Description]: function to receive an array using UART module
 * [Arguments]: it should take two arguments:
 * 1. address of the array in which the received address will be stored
 * 2. the size of the array
 * [returns]: the function returns void
 */

void UART_receiveArray(uint8 *a_dataPtr , uint8 a_arraySize);

void UART_setCallBack(void(*a_ptr)(void));

#endif /* UART_H_ */
