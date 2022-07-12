/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]: timer.h
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DESCRIPTION]: header file for AVR timer module
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#ifndef TIMER_H_
#define TIMER_H_

#include"std_types.h"
#include"common_macros.h"
#include"micro_config.h"

/***************************************************************************************
 *                                 Types Declaration                                   *
 **************************************************************************************/
/*
 * Description: enumeration to select the number of timer to be initialized
 */
typedef enum
{
	timer0,timer1,timer2
}Timer_Select;

/*
 * Description: enumeration to select Clock prescale for the timer
 */
typedef enum
{
	NO_CLOCK, F_CPU_CLOCK , F_CPU_8 , F_CPU_64 , F_CPU_256 , F_CPU_1024,
	F_CPU2_8=2 ,F_CPU2_32,F_CPU2_64,F_CPU2_128,F_CPU2_256,F_CPU2_1024
}Timer_Clock;

/*
 * Description: enumeration to select the mode of the timer
 */
typedef enum
{
	OVF, PWM_PHASE_CORRECT , CTC , FAST_PWM
}Timer_Mode;

/*
 * Description: enumeration to select mode of the compare match mode for
 * timer 0 , timer 2 and channel A in timer 1
 */
typedef enum
{
	NORMAL , TOGGLE , CLEAR , SET , NOT_USING
}Timer_CtcOutput;

/*
 * Description: enumeration to select mode of the compare match mode for channel B in timer  1
 */
typedef enum
{
	NORMAL2 , TOGGLE2 , CLEAR2 , SET2 , NOT_USING2
}Timer_CtcOutputB;

/* Description : structure to store the required information to initialize the timer
 * the structure have 8 members
 * 1. the number of the timer to be initialized
 * 2. set the timer mode
 * 3. prescale of the timers clock
 * 4. set the mode of the compare match mode for timer 0 , timer 2 and channel A in timer 1
 * 5. set the mode of the compare match mode for channel B in timer  1
 * 6. start value of the timer
 * 7. the value of compare match for timer 0 , timer 2 and channel A in timer 1
 * 8. the value of compare match for channel B in timer 1
 */
typedef struct
{
	Timer_Select timer;				 /* number of the timer */
	Timer_Mode mode;   			     /* mode of the timer */
	/* for channel B in timer 1 choose from CPU2 options */
	Timer_Clock clock;				 /* clock prescale */
	Timer_CtcOutput ctc;			 /* mode of compare match mode for timer0,timer2 and channel A in timer 1*/
	Timer_CtcOutputB ctcB;			 /* mode of compare match mode for channel B in timer 1 */
	uint16 s_timerStartValue;		 /* start value of the timer */
	uint16 s_timerCompareValue;		 /* compare match for timer 0 , timer 2 and channel A in timer 1 */
	uint16 s_timerCompareValue2;	 /*compare match for channel B in timer 1*/
}Timer_ConfigType;

/**************************************************************************************
 *                             Functions Prototypes                                   *
 **************************************************************************************/

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
/* the function takes one argument a structure of type : Timer_ConfigType */
void TIMER_init(const Timer_ConfigType *Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to stop the timer.
 * the function takes one argument indicating which timer
 */
void TIMER_stop(uint8 a_timerNumber);


#endif /* TIMER_H_ */
