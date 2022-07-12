/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: timer.h
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: source file for the UART module in AVR Micro controller
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#include"uart.h"
#include<stdio.h>

/******************************************************************************************************
 *                                               Global Variables                                     *
 *****************************************************************************************************/
/* Global variable to hold the data of receiver buffer  */
static volatile void (*g_callBackPtr) (void)=NULL;
/*******************************************************************************************************
 *                                               Interrupt Service Routines                            *
 ******************************************************************************************************/
ISR(USART_RXC_vect)
{
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();  /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************************************
 *                                              Functions Definitions                                  *
 *******************************************************************************************************/

/*
 * Description : Function to initialize the UART Driver
 * 	1.set the value of baud rate
 * 	2.set the required mode for the Receiver of UART
 * 	3.Set the required number of Data bits transmitted or Received
 * 	4.set the required parity bit or disable it
 * 	5.set the required number of stop bits
 * 	6.Initialize it in double speed mode
 */
/* the function takes one argument a structure of type : Uart_ConfigType */
void UART_init(const Uart_ConfigType *Config_Ptr)
{
	/* set U2X to 1: Doubles the USART Transmission Speed */
	SET_BIT(UCSRA,U2X);
	/************************** UCSRB Description **************************
	 * USART RX Complete Interrupt Enable RXCIE=0 if mode is polling and =1 if it is interrupt
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = the third bit of the data selection (function argument )
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB =(1<<RXEN) | (1<<TXEN) | ((Config_Ptr->mode)<<RXCIE);
	UCSRB = (UCSRB & 0xfb) | (Config_Ptr->data & 0x04);
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = from parity selection (function argument )
	 * USBS    = from stop bit selection (function argument )
	 * UCSZ1:0 = form the function argument
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC= (1<<URSEL);
	UCSRC= (UCSRC & 0xcf) | ((Config_Ptr->parity & 0x03)<<4);  /* set the required parity */
	UCSRC= (UCSRC & 0xf7) | ((Config_Ptr->stop & 0x1)<<3);	   /* set the required stop bits */
	UCSRC= (UCSRC & 0xf9) | ((Config_Ptr->data & 0x03)<<1);	   /* set the required number of data bits */
	/* First 8 bits from the BAUD_rate inside UBRRL and last 4 bits in UBRRH*/
	UBRRH=(((F_CPU)/(8 * Config_Ptr->s_baudRate))-1)>>8;
	UBRRL=((F_CPU)/(8 * Config_Ptr->s_baudRate))-1;
}

/* Description: function to send one byte of data using UART module
 * the function should take one argument only which is the required data to be transmitted
 * the argument type is : uint16
 */
void UART_sendByte(uint16 a_data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	/* if it is in 9 bit data mode the ninth bit should be in TXB8 bit in UCSRA register */
	UCSRA= (UCSRA & 0xfe) | ((a_data & 0x0100)>>8);
	UDR= a_data;
}

/* Description: function to receive one byte of data using UART module
 * the function do not take arguments
 * the function should return the received data by UART module
 */
uint16 UART_receiveByte(void){
	/* variable to hold the received data */
	uint16 a_data=0;
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC));
	/* check if there are errors in the frame or parity */
	if(BIT_IS_SET(UCSRA,FE) || BIT_IS_SET(UCSRA,PE)){
		return 0xff;
	}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after reading this data */
	a_data= ((UCSRA& 0x01)<<8) | (UDR); /* take the ninth bit from UCSRA register if it is in nine bit mode */
	return a_data;
}

/* Description: function to send string using UART module
 * the function should take one argument only which is the required string to be transmitted
 * the function should send the string byte by byte using the function UART_sendByte
 */
void UART_sendString(const uint8 *str){
	uint8 i=0;
	/* looping through the string characters and send them one by one */
	while(str[i] != '\0'){
		UART_sendByte(str[i]);
		i++;
	}
}

/* Description: function to receive string using UART module
 * the function takes one argument: the address the string will be stored at
 * the function receives the string byte by byte using the function UART_receiveByte
 */
void UART_receiveString(uint8 *str){
	uint8 i=0;
	str[i]= (uint8)UART_receiveByte(); /*receive the first character */
	/*if the special character '#' is sent that indicates the end of string */
	while(str[i] != '#'){
		i++;
		str[i]=(uint8)UART_receiveByte();
	}
	str[i]=(uint8)'\0'; /* assign the null at the end of string */
}

/*[Description]: function to send an array using UART module
 * [Arguments]: it should take two arguments:
 * 1. address of the array to be sent
 * 2. the size of the array
 * [returns]: the function returns void
 */

void UART_sendArray(uint8 *a_dataPtr , uint8 a_arraySize)
{
	uint8 i=0;
	for(i=0;i< a_arraySize;i++)
	{
		UART_sendByte(*a_dataPtr);
		a_dataPtr++;
	}
}

/*[Description]: function to receive an array using UART module
 * [Arguments]: it should take two arguments:
 * 1. address of the array in which the received address will be stored
 * 2. the size of the array
 * [returns]: the function returns void
 */

void UART_receiveArray(uint8 *a_dataPtr , uint8 a_arraySize)
{
	uint8 i=0;
	for(i=0;i< a_arraySize;i++)
	{
		*a_dataPtr= UART_receiveByte();
		a_dataPtr++;
	}
}

void UART_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}


