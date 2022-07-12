/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]:  hmi_ecu_declerations
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DATE] [CREATED]:  Dec 5, 2020
 *
 * [DESCRIPTION]: header file for the functions of the Human Interface ECU
 * 				  The file contain the Prototypes of functions for HMI_ECU and the Preprocessor macros
 *				   and the Global variables
 -----------------------------------------------------------------------------------------------------------------------------*/

#ifndef HMI_ECU_DECLERATIONS_H_
#define HMI_ECU_DECLERATIONS_H_

#include"lcd.h"
#include"timer.h"
#include"uart.h"
#include"keypad.h"
#include <avr/eeprom.h>


/************************************************************************************************
 * 										PreProcessor Macros										*
 ***********************************************************************************************/
#define NEW_PASSWORD 0x0A
#define PASSWORD_IS_SAVED 0x0B
#define CHANGE_PASSWORD 0x0C
#define OPEN_DOOR 0x0D
#define PASSWORD_IS_RIGHT 0x0E
#define PASSWORD_IS_WRONG 0x0F
#define PRECEDE_CHANGE 0x01
#define DONT_CHANGE 0x02
#define PASSWORD_IS_CHANGED 0x03
#define FIRST_TIME_ADDRESS 0x22

/************************************************************************************************
 * 										Global Variables										*
 ***********************************************************************************************/

/* global variable to indicate the received byte from the CONTROL ECU */
extern volatile uint8 g_select;
/* global variable to count the number of seconds for the Timer Module*/
extern volatile uint8 g_numberOfSeconds;
/* global variable indicating if its the first time for the system or not */
extern volatile uint8 flag;
/* global variable indicating if there is a pressed button */
extern volatile uint8 condition;
/* global variable to count how many times the password was entered wrong */
extern volatile uint8 wrong_counter;

/************************************************************************************************
 * 										  Function Prototypes									*
 ***********************************************************************************************/
/* [Description]: prototype for the call back function for the Receive interrupt of the UART module for HMI ECU
 * 				  it should receive the byte and set the value for g_select variable indicating the mode
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_uartAppProcessing(void);
/* [Description]: prototype for the call back function for the interrupt of Timer Module for the function
 * 				  open door for HMI ECU it should increment the number of seconds  and if the seconds =15 or 18
 * 				  it display a message on the LCD
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_timerOpenDoorProcessing(void);
/* [Description]: prototype for the call back function for the interrupt of Timer Module for the function
 *				 Buzzer on for HMI ECU it should increment the number of seconds
 *				 and if the seconds =60 it stops the buzzer
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_timerBuzzerProcessing(void);
/* [Description]: function responsiple for indicating if two arrays are equal or not
 * [Arguments]: it takes two arrays addresses and the size of the array
 * [Returns]: return 1 if the two arrays are equal
 * 			  return 0 if the two arrays are different
 */
uint8 HMI_compare(uint8 *a_first , uint8 *a_second , uint8 a_arraySize);
/* [Description]: function responsible for displaying the diplaying of main options on LCD
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_displayMainOptions(void);
/* [Description]: function responsible for displaying a message indicating that the system is locked
 * 				  it should display the message for one minute using TIMER module
 * [Arguments]: it takes a structure of type Timer_ConfigType to initialize the timer
 * [Returns]: no return
 */
void HMI_buzzerOnForOneMinute(Timer_ConfigType *Timer_ConfigPtr);
/* [Description]: function responsible for turning the buzzer of an display the main options again
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_buzzerOff(void);
/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and take the password again if the both password are equal send to the CONTROL ECU
 * 				  to set the password and send the password this function should display only one
 * 				  time at the beginning of the sysytem
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_setPassword(void);
/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and send to the CONTROL ECU to change password then send the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_changePassword(void);
/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and send to the CONTROL ECU to open the door then send the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_openDoor (void);
/* [Description]: function responsible for initializing the TIMER module and displaying the open door messages
 * [Arguments]: a structure of typr Timer_ConfigType
 * [Returns]: no return
 */
void HMI_displayOpenDoor(Timer_ConfigType *Timer_ConfigPtr);
/* [Description]: function responsible for initializing the TIMER module and displaying
 * 				  that the entered password is wrong if the entered password is wrong for 3 times
 * 				  it should turn on the Buzzer
 * [Arguments]: a structure of type Timer_ConfigType
 * [Returns]: no return
 */
void HMI_wrongPassword(Timer_ConfigType *Timer_ConfigPtr);
/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and take the password again if the both password are equal send to the CONTROL ECU
 * 				  to set the new password and send the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_precedeChange(void);




#endif /* HMI_ECU_DECLERATIONS_H_ */
