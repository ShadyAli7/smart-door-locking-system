/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]:  hmi_ecu
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DATE] [CREATED]:  Dec 1, 2020
 *
 * [DESCRIPTION]: Human Interface ECU for Door Locker System
 * 				  Using ATmega16 to control a 2x16 LCD module , a 4x4 KeyPad and its connected to
 * 				  the Control ECU through UART module it should display the messages on the LCD
 * 				  and takes input from the KeyPad send it to the Control ECU through UART module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#include "hmi_ecu_declerations.h"

/************************************************************************************************
 * 									    	Main Function							      		*
 ***********************************************************************************************/
int main(void)
{
	/* var: local variable to hold the value of address (0x0d) from the internal EEPROM
	 * indicating if the system has been initialized before
	 * inputChoise: variable to hold the pressed key from KeyPad
	 */
	uint8 var , inputChoise;

	/* Structure holding the required information for the initialization of TIMER module */
	Timer_ConfigType Timer_Config={timer1,CTC,F_CPU_1024,NORMAL,NOT_USING2,0,1000,0};

	/* Structure holding the required information for the initialization of UART module */
	Uart_ConfigType Uart_Config={9600,interrupt,eight_bit,disable,one_bit};

	/* set the call back function for UART when the interrupt flag of receiving =1 */
	UART_setCallBack(HMI_uartAppProcessing);

	LCD_init(); /* initializing LCD module */
	/* initializing the UART module and passing a structure for the required information */
	UART_init(&Uart_Config);

	SET_BIT(SREG,7); /* enabling the Global I-bit */
	/*Read a value (0x05) from address (0x0D) in Internal EEPROM to check if the first time */
	var = eeprom_read_byte(FIRST_TIME_ADDRESS);
	if (var != 0x05)
	{
		/* set the flag variable to 0 cause its the first time for the system */
		flag = 0;
	}
	else if (var == 0x05)
	{
		/* set the flag variable to 1 cause its not the first time for the system */
		flag = 1;
	}

	while(1)
	{
		if(flag==0)
		{
			/* for the first time of the system call the function to set the password for the first time */
			HMI_setPassword();
		}
		else if(flag==1)
		{
			/* when this is not the first time for the system check for the condition variable */
			if(condition == 0)
			{
				/* until there is a pressed key display the main options */
				HMI_displayMainOptions();
				inputChoise= KeyPad_getPressedKey(); /*take the pressed key and store it in inputChoise */
				if(inputChoise == '+')
				{
					/* if the pressed key is '+' call the function to change the password */
					HMI_changePassword();
				}
				else if(inputChoise == '-')
				{
					/* if the pressed key is '-' call the function to open the door  */
					HMI_openDoor();
				}
			}
			else if(condition == 1)
			{
				/* if the condition is one means that there is a pressed key */
				if(g_select==1)
				{
					/* if the entered password is right continue to the displaying of open door messages */
					HMI_displayOpenDoor(&Timer_Config);
				}
				else if(g_select == 2)
				{
					/* if the entered password is wrong call the funtion wrong password */
					HMI_wrongPassword(&Timer_Config);
				}
				else if(g_select == 3)
				{
					/* if the entered password is right continue to taking the new password */
					HMI_precedeChange();
				}
				else if(g_select == 4)
				{
					/* if the entered password is wrong call the funtion wrong password */
					g_select=0;
					HMI_wrongPassword(&Timer_Config);
					condition=0;
				}
			}
		}
	}
}
