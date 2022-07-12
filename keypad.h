/*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: keypad
 *
 * [File Name]: shady_keypad.h
 *
 * [Description]: header file for the keypad driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include"common_macros.h"
#include"micro_config.h"
#include"std_types.h"

/* keypad number of rows and columns*/
#define N_col 4
#define N_row 4

/*keypad port configurations*/
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN PINA
#define KEYPAD_PORT_DIR DDRA

/* prototype for the function responsible for getting the pressed key*/

uint8 KeyPad_getPressedKey(void);


#endif /* KEYPAD_H_ */
