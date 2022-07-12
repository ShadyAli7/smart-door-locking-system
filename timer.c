/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: timer
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: source file for AVR timer module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#include"timer.h"
#include<stdio.h>

/******************************************************************************************************
 *                                                  Global Variables                                  *
 *****************************************************************************************************/
/* Global variable to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr) (void)=NULL;
/* Global variable to hold the address of the second call back function in case using channel B
 * in timer 1 in the application */
static volatile void (*g_callBackPtr2) (void)=NULL;

/*******************************************************************************************************
 *                                               Interrupt Service Routines                            *
 ******************************************************************************************************/
/*
 * Description: Interrupt Service Routine for timer 0 in overflow mode(normal mode)
 */
ISR(TIMER0_OVF_vect)
{
	/* Call the Call Back function in the application after the overflow occurs in timer 0 */
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();  /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*
 * Description: Interrupt Service Routine for timer 0 in compare match mode(CTC normal mode)
 */
ISR(TIMER0_COMP_vect)
{
	/* Call the Call Back function in the application after the compare match occurs in timer 0 */
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();
	}
}

/*
 * Description: Interrupt Service Routine for timer 1 in overflow mode(normal mode)
 */
ISR(TIMER1_OVF_vect)
{
	/* Call the Call Back function in the application after the overflow occurs in timer 1 */
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();
	}
}

/*
 * Description: Interrupt Service Routine for timer 1 in compare match for channel A mode(CTC normal mode)
 */
ISR(TIMER1_COMPA_vect)
{
	/* Call the Call Back function in the application after the compare match in channel A occurs in timer 1 */
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();
	}
}

/*
 * Description: Interrupt Service Routine for timer 1 in compare match for channel B mode(CTC normal mode)
 */
ISR(TIMER1_COMPB_vect)
{
	/* Call the Call Back function in the application after the compare match in channel B occurs in timer 1 */
	if(g_callBackPtr2 != NULL){
		(*g_callBackPtr2)();
	}
}

/*
 * Description: Interrupt Service Routine for timer 2 in overflow mode(normal mode)
 */
ISR(TIMER2_OVF_vect)
{
	/* Call the Call Back function in the application after the overflow occurs in timer 2 */
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();
	}
}

/*
 * Description: Interrupt Service Routine for timer 2 in compare match mode(CTC normal mode)
 */
ISR(TIMER2_COMP_vect)
{
	/* Call the Call Back function in the application after the compare match occurs in timer 2 */
	if(g_callBackPtr != NULL){
		(*g_callBackPtr)();
	}
}

/****************************************************************************************************
 *                                       Functions Definitions                                      *
 ***************************************************************************************************/

/*
 * Description : Function to initialize the Timer driver
 * 	1.select which timer to initialize
 * 	2.set the required mode for the Timer
 * 	3.Set the required clock prescaler.
 * 	4.choose the output compare match mode
 * 	5.set the required timer start value
 * 	6.set the compare value if it is in compare mode
 * 	7.set the compare value for channel B in timer 1
 */
void TIMER_init(const Timer_ConfigType *Config_Ptr)
{
	/* condition to check for the required timer */

	/****************************************************************************************************
	 *                                      timer0 initialization                                       *
	 ***************************************************************************************************/

	if(Config_Ptr->timer == timer0)
	{
		/* condition to enable the OC0 pin as output in case of output compare match
		 * and the mode is toggle or clear or set OC0 on compare match
		 */
		if((Config_Ptr->ctc == TOGGLE) || (Config_Ptr->ctc == CLEAR) || (Config_Ptr->ctc == SET))
		{
			SET_BIT(DDRB,PB3);  /* set OC0 as output pin */
		}
		/* the FOC0 is activated cause it is non-pwm  */
		SET_BIT(TCCR0,FOC0);
		/*
		 * insert the required mode in the two bits : WGM00 , WGM01 OF TCCR0 register
		 */
		TCCR0= (TCCR0 & 0xbf) | ((Config_Ptr->mode & 0x01)<<6);
		TCCR0= (TCCR0 & 0xF7) | ((Config_Ptr->mode & 0x02)<<2);
		/*
		 * insert the required clock value in the first three bits (CS00, CS01 and CS02)
	     * of TCCR0 Register
		 */
		TCCR0= (TCCR0 & 0xf8) | (Config_Ptr->clock & 0x07);
		/*
		 * insert the required compare match output mode in the two bits: COM00,COM01
		 * in the TCCR0 register
		 */
		TCCR0= (TCCR0 & 0xcf) | ((Config_Ptr->ctc & 0x03)<<4);
		/* Initial Value for Timer0 */
		TCNT0= (uint8)Config_Ptr->s_timerStartValue;
		/* the compare value that is continuously compared with the counter value(TCNT0)
		 * the compare value is used in case the mode is CTC
		 */
		OCR0= Config_Ptr->s_timerCompareValue;
		/* condition to determine which interrupt bit to enable in TIMSK register*/
		if(Config_Ptr->mode == OVF)
		{
			SET_BIT(TIMSK,TOIE0);
		}
		else if((Config_Ptr->mode == CTC) && (Config_Ptr->ctc == NORMAL))
		{
			SET_BIT(TIMSK,OCIE0);
		}

	}
	/****************************************************************************************************
	 *                                      timer1 initialization                                       *
	 ***************************************************************************************************/
	else if(Config_Ptr->timer == timer1)
	{
		/* condition to enable the OC1A pin as output in case of output compare match on channel A
		 * or enable OC1B pin as output in case of output compare match on channel B
		 * and the mode is toggle or clear or set OC1A or OC1B on compare match
		 */
		if((Config_Ptr->ctc == TOGGLE) || (Config_Ptr->ctc == CLEAR) || (Config_Ptr->ctc == SET))
		{
			SET_BIT(DDRD,PD5);  /* set OC1A as output pin */
		}
		if((Config_Ptr->ctcB == TOGGLE2) || (Config_Ptr->ctcB == CLEAR2) || (Config_Ptr->ctcB == SET2))
		{
			SET_BIT(DDRD,PD4);  /* set OC1B as output pin */
		}
		/* the FOC1A and FOC1B is activated cause it is non-pwm on channel A and B */
		TCCR1A =(1<<FOC1A)|(1<<FOC1B);
		/*
		 * insert the required compare match output mode for channel A in the two bits: COM1A1,COM1A0
		 * in the TCCR1A register
		 */
		TCCR1A = (TCCR1A & 0x3f) | ((Config_Ptr->ctc & 0x03)<<6);
		/*
		 * insert the required compare match output mode for channel B in the two bits: COM1B1,COM1B0
		 * in the TCCR1A register
		 */
		TCCR1A = (TCCR1A & 0xcf) | ((Config_Ptr->ctcB & 0x03)<<4);
		/*
		 * set the WGM10 , WGM11 in TCCR1A equal to (0,0) cause in normal and CTC modes it will always be 0
		 * set th WGM12 , WGM13 in TCCR1B = (0,0) for normal mode
		 * 									(1,0) for CTC mode
		 */
		TCCR1B = (TCCR1B & 0xe7) | ((Config_Ptr->mode)<<2);
		/*
		 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
	     * of TCCR1B Register
		 */
		TCCR1B= (TCCR1B & 0xf8) | (Config_Ptr->clock & 0x07);
		/* Initial Value for Timer1 */
		TCNT1= (uint8)Config_Ptr->s_timerStartValue;
		/* the compare value that is continuously compared with the counter value(TCNT1)
		 * the compare value is used in case the mode is CTC for channel A or B
		 */
		OCR1A= Config_Ptr->s_timerCompareValue; /* compare value for channel A */

		OCR1B= Config_Ptr->s_timerCompareValue2; /* compare value for channel B */
		/* condition to determine which interrupt bit to enable in TIMSK register*/
		if(Config_Ptr->mode == OVF)
		{
			SET_BIT(TIMSK,TOIE1); /* enable interrupt for overflow mode */
		}
		else if((Config_Ptr->mode == CTC) && (Config_Ptr->ctc == NORMAL))
		{
			SET_BIT(TIMSK,OCIE1A); /* enable interrupt for output compare match for channel A mode */
		}
		if((Config_Ptr->mode == CTC) && (Config_Ptr->ctcB == NORMAL2))
		{
			SET_BIT(TIMSK,OCIE1B);  /* enable interrupt for output compare match for channel B mode */
		}
	}
	/****************************************************************************************************
	 *                                      timer2 initialization                                       *
	 ***************************************************************************************************/
	else if(Config_Ptr->timer == timer2)
	{
		/* condition to enable the OC2 pin as output in case of output compare match
		 * and the mode is toggle or clear or set OC2 on compare match
		 */
		if((Config_Ptr->ctc == TOGGLE) || (Config_Ptr->ctc == CLEAR) || (Config_Ptr->ctc == SET))
		{
			SET_BIT(DDRD,PD7);  /* set OC2 as output pin */
		}
		/* the FOC2 is activated cause it is non-pwm  */
		SET_BIT(TCCR2,FOC2);
		/*
		 * insert the required mode in the two bits : WGM20 , WGM21 OF TCCR2 register
		 */
		TCCR2= (TCCR2 & 0xbf) | ((Config_Ptr->mode & 0x01)<<6);
		TCCR2= (TCCR2 & 0xF7) | ((Config_Ptr->mode & 0x02)<<2);
		/*
		 * insert the required clock value in the first three bits (CS20, CS21 and CS22)
	     * of TCCR2 Register
		 */
		TCCR2= (TCCR2 & 0xf8) | (Config_Ptr->clock & 0x07);
		/*
		 * insert the required compare match output mode in the two bits: COM20,COM21
		 * in the TCCR2 register
		 */
		TCCR2= (TCCR2 & 0xcf) | ((Config_Ptr->ctc & 0x03)<<4);
		/* Initial Value for Timer2 */
		TCNT2= (uint8)Config_Ptr->s_timerStartValue;
		/* the compare value that is continuously compared with the counter value(TCNT2)
		 * the compare value is used in case the mode is CTC
		 */
		OCR2= Config_Ptr->s_timerCompareValue;
		/* condition to determine which interrupt bit to enable in TIMSK register*/
		if(Config_Ptr->mode == OVF)
		{
			SET_BIT(TIMSK,TOIE2); /* enable interrupt for overflow mode */
		}
		else if((Config_Ptr->mode == CTC) && (Config_Ptr->ctc == NORMAL))
		{
			SET_BIT(TIMSK,OCIE2); /* enable interrupt on compare match */
		}
	}
}

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/*
 * Description: Function to stop the timer.
 * the function takes one argument indicating which timer
 */
void TIMER_stop(Timer_Select a_timerNumber)
{
	/* Condition to determine which timer */
	if(a_timerNumber == timer0) /*stopping timer 0*/
	{
		/* clear TCCR0 register to stop and reset timer 0 */
		TCCR0=0;
	}
	else if(a_timerNumber == timer1) /* Stopping timer 1*/
	{
		/* clear TCCR1A and TCCR1B registers to stop and reset timer 1 */
		TCCR1A=0;
		TCCR1B=0;
	}
	else if(a_timerNumber == timer2) /* Stopping timer 2 */
	{
		/* clear TCCR2 register to stop and reset timer 2 */
		TCCR2=0;
	}
}

