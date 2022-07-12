/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]:  control_ecu
 *
 * [AUTHOR]:     Shady Ali
 *
 * [DATE] [CREATED]:  Dec 1, 2020
 *
 * [DESCRIPTION]: Control ECU for Door Locker System
 * 				  Using ATmega16 to control a DC MOTOR using L293D , a Buzzer , External EEPROM and its connected to
 * 				  the Human Interface ECU through UART module . It should take the password from the Human Interface
 * 				  ECU and process it , control the process of openning the door and Buzzer for security
 *
 -----------------------------------------------------------------------------------------------------------------------------*/

#include"control_ecu_declerations.h"


/************************************************************************************************
 * 										    Main Function					   					*
 ***********************************************************************************************/
int main(void)
{
	/* Structure holding the required information for the initialization of TIMER module */
	Timer_ConfigType Timer_Config={timer1,CTC,F_CPU_1024,NORMAL,NOT_USING2,0,1000,0};

	/* Structure holding the required information for the initialization of UART module */
	Uart_ConfigType Uart_Config={9600,interrupt,eight_bit,disable,one_bit};

	/* Structure holding the required information for the initialization of I2C module  */
	Twi_ConfigType Twi_Config={100000 , F_CPU_1 , 1  };

	/* set the call back function for UART when the interrupt flag of receiving =1 */
	UART_setCallBack(CONTROL_uartAppProcessing);

	/* initializing the UART module and passing a structure for the required information */
	UART_init(&Uart_Config);
	/* initializing the external EEPROM with I2C and passing a structure for the required information */
	EEPROM_init(&Twi_Config);
	/* enabling the Global I-bit */
	SET_BIT(SREG,7);
	/* declare the Buzzer pin as output pin */
	SET_BIT(BUZZER_DIR , BUZZER_PIN);
	while(1)
	{
		while(g_select == 0); /* polling until receive an order from the HMI ECU though UART */
		if(g_select == 1)
		{
			g_select=0;
			/* write the password for the first time in the external EEPROM  */
			EEPROM_writeArray(PASSWORD_ADDRESS , received_newPassword);
		}
		else if(g_select == 2)
		{
			/* when the receive byte from the HMI ECU is open door call the
			 * function CONTROL_openDoor
			 */
			CONTROL_openDoor(&Timer_Config);
		}
		else if(g_select == 3)
		{
			/* when the receive byte from the HMI ECU is change password call the
			 * function CONTROL_changePassword
			 */
			CONTROL_changePassword(&Timer_Config);
		}
		else if(g_select == 4)
		{
			/* when the receive byte from the HMI ECU is changed password call the
			 * function CONTROL_setPassword to store the new password
			 */
			CONTROL_setPassword();
		}

	}
}
