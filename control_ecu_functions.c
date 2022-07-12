/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]:  hmi_ecu_functions
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DATE] [CREATED]:  Dec 5, 2020
 *
 * [DESCRIPTION]: source file for the functions of the Human Interface ECU
 * 				  The file contain the definitions of functions for HMI_ECU and the Global variables
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#include"control_ecu_declerations.h"

/************************************************************************************************
 * 										Global Variables										*
 ***********************************************************************************************/

/* global variable to indicate the received byte from the CONTROL ECU */
volatile uint8 g_select=0;
/* global variable to count the number of seconds for the Timer Module*/
volatile uint8 g_numberOfSeconds=0;
/* global array to hold the new password for the first time in the system */
volatile uint8 received_newPassword[5];
/* global array to hold temporery password to be checked */
volatile uint8 received_tempPassword[5];
/* global array to hold received password to change the existing one */
volatile uint8 received_changePassword[5];
/* global array to store the real password from the external EEPROM at it */
volatile uint8 real_password[5];
/* global variable to count how many times the password was entered wrong */
volatile uint8 wrong_counter=0;

/************************************************************************************************
 * 										  Function Definitions									*
 ***********************************************************************************************/

/* [Description]: the call back function for the Receive interrupt of the UART module for CONTROL ECU
 * 				  it should receive the byte and set the value for g_select variable indicating the mode
 * 				  and then receive the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_uartAppProcessing(void)
{
	/* variable to hold the received byte indicating which mode */
	uint8 key = UART_receiveByte();
	/* condition to select the mode of CONTROL ECU */
	if(key == NEW_PASSWORD)
	{
		/* set the global variable g_select to 1 to if the HMI ECU wants to set new password */
		g_select=1;
		/* receive the new password */
		UART_receiveArray(received_newPassword ,5);
	}
	else if(key == OPEN_DOOR)
	{
		/* set the global variable g_select to 2 the HMI ECU wants to open the door */
		g_select=2;
		/* receive input password from the HMI ECU */
		UART_receiveArray(received_tempPassword , 5);
	}
	else if(key == CHANGE_PASSWORD)
	{
		/* set the global variable g_select to 3 if the HMI ECU wants to change the password */
		g_select=3;
		/* receive input password from the HMI ECU */
		UART_receiveArray(received_tempPassword , 5);
	}
	else if(key == PASSWORD_IS_CHANGED)
	{
		/* set the global variable g_select to 4 if the password is changed from the HMI ECU */
		g_select=4;
		/* receive the changed password */
		UART_receiveArray(received_changePassword , 5);
	}
}

/* [Description]: the call back function for the interrupt of Timer Module for the function open door for CONTROL ECU
 * 				  it should increment the number of seconds  and if the seconds =15 stops the motor
 * 				  and if it = 18 turn on the motor in the reverse direction
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void Control_timerOpenDoorProcessing(void)
{
	/* increment the seconds when the flag of Timer =1 */
	g_numberOfSeconds++;
	if(g_numberOfSeconds==15)
	{
		/* at the 15 second stop the motor */
		DCMOTOR_stop();
	}
	else if(g_numberOfSeconds==18)
	{
		/* at the 18 second start the motor in reverse direction */
		DCMOTOR_acw();
	}
}

/* [Description]: the call back function for the interrupt of Timer Module for the function Buzzer on for CONTROL ECU
 * 				  it should increment the number of seconds  and if the seconds =60 it stops the buzzer
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_timerBuzzerProcessing(void)
{
	/* increment the seconds when the flag of Timer =1 */
	g_numberOfSeconds++;
	if(g_numberOfSeconds == 60)
	{
		/* after one minute turn off the buzzer */
		CONTROL_buzzerOff();
	}
}

/* [Description]: function responsible for indicating if two arrays are equal or not
 * [Arguments]: it takes two arrays addresses and the size of the array
 * [Returns]: return 1 if the two arrays are equal
 * 			  return 0 if the two arrays are different
 */
uint8 CONTROL_compare(uint8 *a_first , uint8 *a_second , uint8 a_arraySize)
{
	/* incremental variable for the for loop */
	uint8 i=0;
	for(i=0;i<a_arraySize;i++)
	{
		/* condition to compare each elements of the two arrays */
		if(a_first[i] != a_second[i])
		{
			return 0;
		}
	}
	return 1;
}

/* [Description]: function responsible for initializing the Timer module and turn on the Buzzer
 * [Arguments]: it takes a structure of type Timer_ConfigType to initialize the timer
 * [Returns]: no return
 */
void CONTROL_buzzerOnForOneMinute(Timer_ConfigType *Timer_ConfigPtr)
{
	BUZZER_DIR |=(1<<BUZZER_PIN); /* initialize the buzzer pin as output pin */
	/* initialize the Timer and give it a structure holding the required information */
	TIMER_init(Timer_ConfigPtr);
	/* set the call back function for the Timer module */
	TIMER_setCallBack(CONTROL_timerBuzzerProcessing);
	g_numberOfSeconds=0;
	BUZZER_PORT |=(1<<BUZZER_PIN);  /* turn on the Buzzer */
	while(g_numberOfSeconds != 60); /* polling for one minute */
}

/* [Description]: function responsible for turning the buzzer off
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_buzzerOff(void)
{
	BUZZER_PORT &= (~(1<<BUZZER_PIN)); /* turn off the buzzer */
}


/* [Description]: function responsible for comparing the received password from the HMI ECU with the real password
 * 				  if it is equal send to the HMI ECU that password is right , initialize Timer module
 * 				  and turn on the motor in clock wise direction then stop the motor after 33 seconds
 * 				  if its wrong send to the HMI ECU that password is wrong and increment the wrong counter
 * 				  when the wrong ==3 turn on the Buzzer for one minute
 * [Arguments]: structure of type Timer_ConfigType holding the required information for Timer
 * [Returns]: no return
 */
void CONTROL_openDoor(Timer_ConfigType *Timer_ConfigPtr)
{
	/* variable to hold the result of comparision */
	uint8 temp;
	g_select=0;
	/* read the real password from the external  EEPROM and store it in the array real_password */
	EEPROM_readArray(PASSWORD_ADDRESS , real_password);
	/* compare between the entered password and the real one  ,and store the result in temp variable */
	temp= CONTROL_compare(received_tempPassword,real_password , 5);
	if(temp == 1)
	{
		/* send to the HMI ECU that the entered password is right */
		UART_sendByte(PASSWORD_IS_RIGHT);
		TIMER_init(Timer_ConfigPtr); /* initialize the Timer */
		TIMER_setCallBack(Control_timerOpenDoorProcessing); /* set the call back function for the Timer module */
		g_numberOfSeconds=0;
		DCMOTOR_cw(); /* turn on the motor in clock wise direction to open the door */
		while(g_numberOfSeconds != 33);/* polling for the required time for the door to open and close */
		g_numberOfSeconds=0;
		DCMOTOR_stop(); /* stop the motor after 33 second*/
		TIMER_stop(Timer_ConfigPtr->timer); /* stop the Timer */
	}
	else if(temp == 0)
	{
		/* if the entered password does not match with the real password send to the HMI ECU  that password is wrong*/
		UART_sendByte(PASSWORD_IS_WRONG);
		/* increment the wrong counter */
		wrong_counter++;
		if(wrong_counter==3)
		{
			/* if the entered password is wrong for 3 times activate the Buzzer for one minute */
			CONTROL_buzzerOnForOneMinute(Timer_ConfigPtr);
			wrong_counter=0;
		}
	}
}

/* [Description]: function responsible for comparing the received password with the real password
 * 				  if it is right send to the HMI ECU to precede change
 * 				  and if it is wrong send to the HMI ECU not to change and increase the wrong counter
 * [Arguments]: structure of type Timer_ConfigType holding the required information for Timer
 * [Returns]: no return
 */
void CONTROL_changePassword(Timer_ConfigType *Timer_ConfigPtr)
{
	/* variable to hold the result of comparision */
	uint8 temp;
	g_select=0;
	/* read the real password from the external  EEPROM and store it in the array real_password */
	EEPROM_readArray(PASSWORD_ADDRESS , real_password);
	/* compare between the entered password and the real one  ,and store the result in temp variable */
	temp= CONTROL_compare(received_tempPassword,real_password, 5);
	if(temp == 1)
	{
		/* if the entered password matches the real password send to the HMI ECU to precede change */
		UART_sendByte(PRECEDE_CHANGE);
	}
	else if(temp == 0)
	{
		/* if the entered password does not matches the real password send to the HMI ECU to dont change*/
		UART_sendByte(DONT_CHANGE);
		wrong_counter++; /* increment the wrong counter */
		if(wrong_counter == 3)
		{
			/* if the entered password is wrong for 3 times activate the Buzzer for one minute */
			CONTROL_buzzerOnForOneMinute(Timer_ConfigPtr);
			wrong_counter=0;
		}
	}
}

/* [Description]: function responsible for writing the password to the external EEPROM
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void CONTROL_setPassword(void)
{
	g_select=0;
	/* write the new password to the external EEPROM */
	EEPROM_writeArray(PASSWORD_ADDRESS ,received_changePassword );

}


