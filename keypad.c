/*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: keypad
 *
 * [File Name]: shady_keypad.c
 *
 * [Description]: source file for the keypad driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/
#include "keypad.h"

#if(N_col==3)
/* the function which is responsible for mapping the pressed switch key in
 * the keypad to its corresponding number in the 4x3 keypad
 */
static uint8 KeyPad_4x3_adjustKeyNumber(uint8 a_buttonNumber);
#elif(N_col==4)
/* the function which is responsible for mapping the pressed switch key in
 * the keypad to its corresponding number in the 4x4 keypad
 */
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 a_buttonNumber);
#endif

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: KeyPad_getPressedKey
 *
 * [Description]: take the button number for the pressed switch
 *
 * [Args]: none
 *
 * [Returns]: uint8 number representing the pressed number of the keypad
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
uint8 KeyPad_getPressedKey(void){
	/* the parameters which is the number of rows and columns*/
	uint8 col,row;
	while(1){
		for(col=0;col<N_col;col++){
			/*only on of the columns will be output and the rest will be input*/
			KEYPAD_PORT_DIR=(0b00010000<<col);
			/* set the output pin to low and enable the internal pull up resistor for row pins*/
			KEYPAD_PORT_OUT=(~(0b00010000<<col));
			/*looping on the rows to detect the pressed switch*/
			for(row=0;row<N_row;row++){
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row)){ /*if the switch in this row*/
					#if(N_col==3)
						return KeyPad_4x3_adjustKeyNumber((row*N_col)+col+1);
					#elif(N_col==4)
						return KeyPad_4x4_adjustKeyNumber((row*N_col)+col+1);
					#endif
				}
			}
		}
	}
}
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: KeyPad_4x3_adjustKeyNumber
 *
 * [Description]: getting the pressed number on the keypad from the switch number
 *
 * [Args]: uint8 a_buttonNumber:
 * 				this argument presents the number of the pressed switch
 *
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#if(N_col==3)

static uint8 KeyPad_4x3_adjustKeyNumber(uint8 a_buttonNumber){
	switch(a_buttonNumber)
	{
	case 10: return'*'; /* ASCII for * */
			 break;
	case 11: return 0;
			 break;
	case 12: return '#'; /*ASCII for # */
			 break;
	default: return a_buttonNumber;
	}
}
#elif(N_col==4)

static uint8 KeyPad_4x4_adjustKeyNumber(uint8 a_buttonNumber){
	switch(a_buttonNumber)
		{
			case 1: return 7;
					break;
			case 2: return 8;
					break;
			case 3: return 9;
					break;
			case 4: return '%'; // ASCII Code of %
					break;
			case 5: return 4;
					break;
			case 6: return 5;
					break;
			case 7: return 6;
					break;
			case 8: return '*'; /* ASCII Code of '*' */
					break;
			case 9: return 1;
					break;
			case 10: return 2;
					break;
			case 11: return 3;
					break;
			case 12: return '-'; /* ASCII Code of '-' */
					break;
			case 13: return 13;  /* ASCII of Enter */
					break;
			case 14: return 0;
					break;
			case 15: return '='; /* ASCII Code of '=' */
					break;
			case 16: return '+'; /* ASCII Code of '+' */
					break;
			default: return a_buttonNumber;
		}
	}

#endif


