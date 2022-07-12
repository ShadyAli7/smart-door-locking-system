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

#include "hmi_ecu_declerations.h"

/************************************************************************************************
 * 										Global Variables										*
 ***********************************************************************************************/

/* global variable to indicate the received byte from the CONTROL ECU */
volatile uint8 g_select=0;
/* global variable to count the number of seconds for the Timer Module*/
volatile uint8 g_numberOfSeconds=0;
/* global variable indicating if its the first time for the system or not */
volatile uint8 flag=0;
/* global variable indicating if there is a pressed button */
volatile uint8 condition=0;
/* global variable to count how many times the password was entered wrong */
volatile uint8 wrong_counter=0;




/************************************************************************************************
 * 										  Function Definitions									*
 ***********************************************************************************************/
/* [Description]: the call back function for the Receive interrupt of the UART module for HMI ECU
 * 				  it should receive the byte and set the value for g_select variable indicating the mode
 * [Arguments]: no arguments
 * [Returns]: no return
 */

void HMI_uartAppProcessing(void)
{
	/* variable to hold the received byte indicating which mode */
	uint8 key =UART_receiveByte();
	/* condition to select the mode of HMI ECU */
	if(key == PASSWORD_IS_RIGHT)
	{
		g_select=1; /* set the global variable g_select to 1 if the password is right */
	}
	else if(key == PASSWORD_IS_WRONG)
	{
		g_select=2; /* set the global variable g_select to 2 if the password is wrong */
	}
	else if(key == PRECEDE_CHANGE)
	{
		/* set the global variable g_select to 3 if the sent password is right
		 * and we can precede to change the password  */
		g_select=3;
	}
	else if(key == DONT_CHANGE)
	{
		/* set the global variable g_select to 4 if the sent password is wrong
		 * and we dont change the password  */
		g_select=4;
	}
}

/* [Description]: the call back function for the interrupt of Timer Module for the function open door for HMI ECU
 * 				  it should increment the number of seconds  and if the seconds =15 or 18
 * 				  it display a message on the LCD
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_timerOpenDoorProcessing(void)
{
	g_numberOfSeconds++; /* increment the global variable holding the number of seconds */
	if(g_numberOfSeconds == 15)
	{
		/* if the seconds is 15 it displays that the door is open */
		LCD_clearScreen();
		LCD_displayString("Door is open");
	}
	else if(g_numberOfSeconds == 18)
	{
		/* if the seconds is 18 it displays that the door locking */
		LCD_clearScreen();
		LCD_displayString("Door is locking");
	}
}

/* [Description]: the call back function for the interrupt of Timer Module for the function Buzzer on for HMI ECU
 * 				  it should increment the number of seconds  and if the seconds =60 it stops the buzzer
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_timerBuzzerProcessing(void)
{
	g_numberOfSeconds++;  /* increment the global variable holding the number of seconds */
	if(g_numberOfSeconds == 60)
	{
		/* after one minute it stops the buzzer and displays main options */
		HMI_buzzerOff();
	}
}

/* [Description]: function responsible for indicating if two arrays are equal or not
 * [Arguments]: it takes two arrays addresses and the size of the array
 * [Returns]: return 1 if the two arrays are equal
 * 			  return 0 if the two arrays are different
 */
uint8 HMI_compare(uint8 *a_first , uint8 *a_second , uint8 a_arraySize)
{
	uint8 i=0; /* incremental variable for the for loop */
	for(i=0;i< a_arraySize;i++)
	{
		/* condition to compare each elements of the two arrays */
		if(a_first[i] != a_second[i])
		{
			return 0;
		}
	}
	return 1;
}

/* [Description]: function responsible for displaying the diplaying of main options on LCD
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_displayMainOptions(void)
{
	LCD_clearScreen();
	LCD_displayString("+ : Change Pass");
	LCD_goToRowColumn(1,0);
	LCD_displayString("- : Open Door");
}

/* [Description]: function responsible for displaying a message indicating that the system is locked
 * 				  it should display the message for one minute using TIMER module
 * [Arguments]: it takes a structure of type Timer_ConfigType to initialize the timer
 * [Returns]: no return
 */
void HMI_buzzerOnForOneMinute(Timer_ConfigType *Timer_ConfigPtr)
{
	/* initialize the Timer and pass the structure holding the required information */
	TIMER_init(Timer_ConfigPtr);
	TIMER_setCallBack(HMI_timerBuzzerProcessing); /* set the call back function for the Timer module */
	g_numberOfSeconds=0;
	/* display the message indicating that there is a thief */
	LCD_clearScreen();
	LCD_displayString("System is Locked");
	LCD_goToRowColumn(1,0);
	LCD_displayString("catch thief!!!");
	while(g_numberOfSeconds != 60); /* polling for one minute */

}

/* [Description]: function responsible for turning the buzzer of an display the main options again
 * [Arguments]: no arguments
 * [Returns]: no return
 */
void HMI_buzzerOff(void)
{
	HMI_displayMainOptions();
}

/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and take the password again if the both password are equal send to the CONTROL ECU
 * 				  to set the password and send the password this function should display only one
 * 				  time at the beginning of the sysytem
 * [Arguments]: no arguments
 * [Returns]: no return
 */

void HMI_setPassword(void)
{
	/* key variable to hold the pressed key from the key pad
	 * i variable is the incremental variable for the for loops
	 */
	uint8 i=0,key;
	/* an array to hold the entered password for the first time */
	uint8 password[5];
	/* an array to hold the entered password for the second time */
	uint8 password2[5];
	LCD_clearScreen();
	do
	{
		LCD_clearScreen();
		LCD_displayString("Enter New Pass:"); /* display a message to enter the password */
		LCD_goToRowColumn(1,6);
		for(i=0;i<5;i++)
		{
			key=KeyPad_getPressedKey();
			if((key >= 0) && (key <= 9))
			{
				/* if the pressed key is a number store it in the password array */
				password[i]=key;
				LCD_displayCharacter('*');
				_delay_ms(350);
			}
			else if(key==13)
			{
				/* if the pressed key is 'enter' repeat the process from the beginning*/
				i=-1;
				LCD_clearScreen();
				LCD_displayString("Enter New Pass:");
				LCD_goToRowColumn(1,6);
				continue;
			}
			else
			{
				/* if the pressed key is not a number or 'enter' dont store it */
				i -=1;
				continue;
			}
		}
		LCD_clearScreen();
		_delay_ms(700);
		LCD_displayString("Enter Pass again:"); /* display a message to enter the password for the second time */
		LCD_goToRowColumn(1,6);
		key=0;
		/* loop for taking the password for the second time */
		for(i=0;i<5;i++)
		{
			key=KeyPad_getPressedKey();
			if((key >= 0) && (key <= 9))
			{
				/* if the pressed key is a number store it in the second password array */
				password2[i]=key;
				LCD_displayCharacter('*');
				_delay_ms(350);
			}
			else if(key==13)
			{
				/* if the pressed key is 'enter' repeat the process from the beginning*/
				i= -1;
				LCD_clearScreen();
				LCD_displayString("Enter Pass again:");
				LCD_goToRowColumn(1,6);
				continue;
			}
			else
			{
				/* if the pressed key is not a number or 'enter' dont store it */
				i -=1;
				continue;
			}
		} /* compare between the two password if they are equal get out off the loop
			*  and if not continue in the loop */
	}while(HMI_compare(password,password2 , 5) == 0);
	LCD_clearScreen();
	LCD_displayString("Password is set");
	_delay_ms(700);
	/* write one byte to the internal EEPROM of the HMI ECU to indicate that the password is set
	 * to not repeat this function again
	 */
	eeprom_write_byte(FIRST_TIME_ADDRESS, 0x05);
	/* set the flag variable to one so this function do not be repeated */
	flag = 1;
	/* send one byte to the CONTROL ECU indicating to store a new password */
	UART_sendByte(NEW_PASSWORD);
	/* send the password to the CONTROL ECU */
	UART_sendArray(password , 5);
}

/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and send to the CONTROL ECU to change password then send the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */

void HMI_changePassword(void)
{
	/* an array to hold the entered password  */
	uint8 password[5];
	/* key variable to hold the pressed key from the key pad
	 * i variable is the incremental variable for the for loops
	 */
	uint8 key=0 , i=0;
	/* set the global variable condition to 1 indicating that the password has been sent */
	condition=1;
	LCD_clearScreen();
	LCD_displayString("Enter Password:"); /* display message to enter the password */
	LCD_goToRowColumn(1,6);
	for(i=0;i<5;i++)
	{
		/* if the pressed key is a number store it in the second password array */
		key=KeyPad_getPressedKey();
		if((key >= 0) && (key <= 9))
		{
			password[i]=key;
			LCD_displayCharacter('*');
			_delay_ms(350);
		}
		else if(key==13)
		{
			/* if the pressed key is 'enter' repeat the process from the beginning*/
			i= -1;
			LCD_clearScreen();
			LCD_displayString("Enter current Pass:");
			LCD_goToRowColumn(1,6);
			continue;
		}
		else
		{
			/* if the pressed key is not a number or 'enter' dont store it */
			i -=1;
			continue;
		}
	}
	/* send one byte to the Control ECU  indicating to change the password */
	UART_sendByte(CHANGE_PASSWORD);
	/* send the password to the Control ECU */
	UART_sendArray(password , 5);
}

/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and send to the CONTROL ECU to open the door then send the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */

void HMI_openDoor (void)
{
	/* an array to hold the entered password  */
	uint8 password[5];
	/* key variable to hold the pressed key from the key pad
	 * i variable is the incremental variable for the for loops
	 */
	uint8 key , i;
	/* set the global variable condition to 1 indicating that the password has been sent */
	condition=1;
	LCD_clearScreen();
	LCD_displayString("Enter Password:"); /* display message to enter the password */
	LCD_goToRowColumn(1,6);
	for(i=0;i<5;i++)
	{
		/* if the pressed key is a number store it in the second password array */
		key=KeyPad_getPressedKey();
		if((key >= 0) && (key <= 9))
		{
			password[i]=key;
			LCD_displayCharacter('*');
			_delay_ms(350);
		}
		else if(key==13)
		{
			/* if the pressed key is 'enter' repeat the process from the beginning*/
			i=-1;
			LCD_clearScreen();
			LCD_displayString("Enter Password:");
			LCD_goToRowColumn(1,6);
			continue;
		}
		else
		{
			/* if the pressed key is 'enter' repeat the process from the beginning*/
			i -=1;
			continue;
		}
	}
	/* send one byte to the Control ECU  indicating to open the door */
	UART_sendByte(OPEN_DOOR);
	/* send the password to the Control ECU */
	UART_sendArray(password , 5);
}

/* [Description]: function responsible for initializing the TIMER module and displaying the open door messages
 * [Arguments]: a structure of typr Timer_ConfigType
 * [Returns]: no return
 */
void HMI_displayOpenDoor(Timer_ConfigType *Timer_ConfigPtr)
{
	/* set the global variable g_select to 0 indicating that the operation has been done*/
	g_select=0;
	TIMER_init(Timer_ConfigPtr); /* initializing Timer with the required information */
	g_numberOfSeconds=0;
	TIMER_setCallBack(HMI_timerOpenDoorProcessing); /*set the call back function of the TIMER */
	LCD_clearScreen();
	LCD_displayString("Door is ");
	LCD_goToRowColumn(1,0);
	LCD_displayString("Unlocking");
	while(g_numberOfSeconds != 33); /* polling for the required time for the door to open and close */
	TIMER_stop(Timer_ConfigPtr->timer); /* stopping the Timer */
	g_numberOfSeconds=0;
	condition=0;
}

/* [Description]: function responsible for initializing the TIMER module and displaying
 * 				  that the entered password is wrong if the entered password is wrong for 3 times
 * 				  it should turn on the Buzzer
 * [Arguments]: a structure of type Timer_ConfigType
 * [Returns]: no return
 */
void HMI_wrongPassword(Timer_ConfigType *Timer_ConfigPtr)
{
	g_select=0;
	wrong_counter++;
	LCD_clearScreen();
	LCD_displayString("Wrong Password");
	_delay_ms(1000);
	if(wrong_counter==3)
	{
		HMI_buzzerOnForOneMinute(Timer_ConfigPtr);
		g_numberOfSeconds=0;
		wrong_counter=0;
		condition=0;
	}
	condition=0;
}

/* [Description]: function responsible for taking the password from the KeyPad module
 * 				  and take the password again if the both password are equal send to the CONTROL ECU
 * 				  to set the new password and send the password
 * [Arguments]: no arguments
 * [Returns]: no return
 */

void HMI_precedeChange(void)
{
	/* key variable to hold the pressed key from the key pad
	 * i variable is the incremental variable for the for loops
	 */
	uint8 i=0,key;
	/* an array to hold the entered password for the first time */
	uint8 password[5];
	/* an array to hold the entered password for the second time */
	uint8 password2[5];
	g_select=0;
	LCD_clearScreen();
	do
	{
		LCD_clearScreen();
		LCD_displayString("Enter New Pass:");/* display a message to enter the new password */
		LCD_goToRowColumn(1,6);
		for(i=0;i<5;i++)
		{
			key=KeyPad_getPressedKey();
			if((key >= 0) && (key <= 9))
			{
				/* if the pressed key is a number store it in the password array */
				password[i]=key;
				LCD_displayCharacter('*');
				_delay_ms(350);
			}
			else if(key==13)
			{
				/* if the pressed key is 'enter' repeat the process from the beginning*/
				i=-1;
				LCD_clearScreen();
				LCD_displayString("Enter New Pass:");
				LCD_goToRowColumn(1,6);
				continue;
			}
			else
			{
				/* if the pressed key is not a number or 'enter' dont store it */
				i -=1;
				continue;
			}
		}
		LCD_clearScreen();
		_delay_ms(700);
		LCD_displayString("Enter Pass again:");/* display a message to enter the password for the second time */
		LCD_goToRowColumn(1,6);
		key=0;
		/* loop for taking the password for the second time */
		for(i=0;i<5;i++)
		{
			key=KeyPad_getPressedKey();
			if((key >= 0) && (key <= 9))
			{
				/* if the pressed key is a number store it in the password array */
				password2[i]=key;
				LCD_displayCharacter('*');
				_delay_ms(550);
			}
			else if(key==13)
			{
				/* if the pressed key is 'enter' repeat the process from the beginning*/
				i=-1;
				LCD_clearScreen();
				LCD_displayString("Enter Pass again:");
				LCD_goToRowColumn(1,6);
				continue;
			}
			else
			{
				/* if the pressed key is not a number or 'enter' dont store it */
				i -=1;
				continue;
			}
		}
		/* compare between the two password if they are equal get out off the loop
		 *  and if not continue in the loop */
	}while(HMI_compare(password,password2 , 5) == 0);
	LCD_clearScreen();
	LCD_displayString("Password is set");
	_delay_ms(700);
	/* send one byte to the CONTROL ECU indicating that the password is changed */
	UART_sendByte(PASSWORD_IS_CHANGED);
	/* send the password to the CONTROL ECU */
	UART_sendArray(password , 5);
	condition=0;
}

