/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]:  hmi_ecu_declerations
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DATE] [CREATED]:  Dec 6, 2020
 *
 * [DESCRIPTION]: header file for the functions of the Control ECU
 * 				  The file contain the Prototypes of functions for HMI_ECU and the Preprocessor macros
 * 				  and the Global variables
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#ifndef CONTROL_ECU_DECLERATIONS_H_
#define CONTROL_ECU_DECLERATIONS_H_

#include"twi.h"
#include"eeprom.h"
#include"timer.h"
#include"dcmotor.h"
#include"uart.h"


/************************************************************************************************
 * 										  PreProcessor Macros									*
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
#define PASSWORD_ADDRESS 0x0090

#define BUZZER_PORT PORTC
#define BUZZER_DIR DDRC
#define BUZZER_PIN PC7

/************************************************************************************************
 * 										Global Variables										*
 ***********************************************************************************************/

/* global variable to indicate the received byte from the CONTROL ECU */
extern volatile uint8 g_select;
/* global variable to count the number of seconds for the Timer Module*/
extern volatile uint8 g_numberOfSeconds;
/* global array to hold the new password for the first time in the system */
extern volatile uint8 received_newPassword[5];
/* global array to hold temporery password to be checked */
extern volatile uint8 received_tempPassword[5];
/* global array to hold received password to change the existing one */
extern volatile uint8 received_changePassword[5];
/* global array to store the real password from the external EEPROM at it */
extern volatile uint8 real_password[5];
/* global variable to count how many times the password was entered wrong */
extern volatile uint8 wrong_counter;


/************************************************************************************************
 * 										  Function Prototypes									*
 ***********************************************************************************************/

/* [Description]: the call back function for the Receive interrupt of the UART module for CONTROL ECU
 * 				  it should receive the byte and set the value for g_select variable indicating the mode
 * 				  and then receive the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_uartAppProcessing(void);
/* [Description]: the call back function for the interrupt of Timer Module for the function open door for CONTROL ECU
 * 				  it should increment the number of seconds  and if the seconds =15 stops the motor
 * 				  and if it = 18 turn on the motor in the reverse direction
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void Control_timerOpenDoorProcessing(void);
/* [Description]: the call back function for the interrupt of Timer Module for the function Buzzer on for CONTROL ECU
 * 				  it should increment the number of seconds  and if the seconds =60 it stops the buzzer
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_timerBuzzerProcessing(void);
/* [Description]: function responsible for indicating if two arrays are equal or not
 * [Arguments]: it takes two arrays addresses and the size of the array
 * [Returns]: return 1 if the two arrays are equal
 * 			  return 0 if the two arrays are different
 */
uint8 CONTROL_compare(uint8 *a_first , uint8 *a_second , uint8 a_arraySize);
/* [Description]: function responsible for initializing the Timer module and turn on the Buzzer
 * [Arguments]: it takes a structure of type Timer_ConfigType to initialize the timer
 * [Returns]: no return
 */
void CONTROL_buzzerOnForOneMinute(Timer_ConfigType *Timer_ConfigPtr);
/* [Description]: function responsible for turning the buzzer off
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_buzzerOff(void);
/* [Description]: function responsible for comparing the received password from the HMI ECU with the real password
 * 				  if it is equal send to the HMI ECU that password is right , initialize Timer module
 * 				  and turn on the motor in clock wise direction then stop the motor after 33 seconds
 * 				  if its wrong send to the HMI ECU that password is wrong and increment the wrong counter
 * 				  when the wrong ==3 turn on the Buzzer for one minute
 * [Arguments]: structure of type Timer_ConfigType holding the required information for Timer
 * [Returns]: no return
 */
void CONTROL_openDoor(Timer_ConfigType *Timer_ConfigPtr);
/* [Description]: function responsible for comparing the received password with the real password
 * 				  if it is right send to the HMI ECU to precede change
 * 				  and if it is wrong send to the HMI ECU not to change and increase the wrong counter
 * [Arguments]: structure of type Timer_ConfigType holding the required information for Timer
 * [Returns]: no return
 */
void CONTROL_changePassword(Timer_ConfigType *Timer_ConfigPtr);
/* [Description]: function responsible for writing the password to the external EEPROM
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_setPassword(void);



#endif /* CONTROL_ECU_DECLERATIONS_H_ */
