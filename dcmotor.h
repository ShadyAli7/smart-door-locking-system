 /*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: DC Motor
 *
 * [File Name]: dcmotor.h
 *
 * [Description]: header file for DC Motor driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include"common_macros.h"
#include"micro_config.h"
#include"std_types.h"

#define ENABLE PB3    		       /* the enable pin */
#define INPUT1 PB4  	 		  /* input1 pin */
#define INPUT2 PB5   		     /* input2 pin */

#define ENABLE_PORT PORTB       /* the port register of the enable pin */
#define INPUT_PORT PORTB		/* the port register of input pins */
#define INPUT_PORT_DIR DDRB     /* the port direction of input pins */

void DCMOTOR_cw(void);		 /* function prototype for rotating the motor clock wise */
void DCMOTOR_acw(void);      /* function prototype for rotating the motor anti clock wise */
void DCMOTOR_stop(void);     /* function prototype for stopping the motor from rotating */

#endif /* DCMOTOR_H_ */
