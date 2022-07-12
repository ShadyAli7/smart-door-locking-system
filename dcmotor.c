  /*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: DC Motor
 *
 * [File Name]:dcmotor.c
 *
 * [Description]: source file for DC Motor Driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/
#include"dcmotor.h"

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: DCMOTOR_cw
 * [Description]:   function responsible for intializing DC Motor to rotate in clock wise
 * [Args]: void
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void DCMOTOR_cw(void){
	INPUT_PORT_DIR|=(1<<INPUT1)|(1<<INPUT2);    /* intializing input1 and input2 as output pins*/
	INPUT_PORT &=(~(1<<INPUT1))&(~(1<<INPUT2)); /* intializing input1 and input2 pins to zero */
	/* in order to rotate the motor in clock wise we need to assign input1 to 0
	 * and input2 to 1*/
	CLEAR_BIT(INPUT_PORT,INPUT1);  /*assign input1 to 0 */
	SET_BIT(INPUT_PORT,INPUT2);   /* assign input2 to 1 */
}

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: DCMOTOR_acw
 * [Description]:   function responsible for intializing DC Motor to rotate in anti clock wise
 * [Args]: void
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void DCMOTOR_acw(void){
	INPUT_PORT_DIR|=(1<<INPUT1)|(1<<INPUT2);    /* intializing input1 and input2 as output pins*/
	INPUT_PORT &=(~(1<<INPUT1))&(~(1<<INPUT2)); /* intializing input1 and input2 pins to zero */
	/* in order to rotate the motor in anti clock wise we need to assign input1 to 1
	 * and input2 to 0       */
	SET_BIT(INPUT_PORT,INPUT1);    /* assign input1 to 1 */
	CLEAR_BIT(INPUT_PORT,INPUT2);  /* assign input2 to 0 */
}

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: DCMOTOR_stop
 * [Description]:   function responsible for stopping the motor from rotating
 * [Args]: void
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void DCMOTOR_stop(void){
	/* to stop the motor input1 and input2 have to be 0  */
	INPUT_PORT &=(~(1<<INPUT1))&(~(1<<INPUT2));   /* assign input1 and input2 to 0  */
}

