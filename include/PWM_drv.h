/************************************************************** 
* FILE:         PWM_drv.c
* DESCRIPTION:  PWM driver for TMS320F28069
****************************************************************/
#ifndef     __PWM_DRV_H__
#define     __PWM_DRV_H__

#include    "DSP28x_Project.h"

#include    "define.h"
#include    "globals.h"

/* Definitions */
// peripherals on DEBUG_STOP:
// 0 stop immediately, 1 stop when finished, 2 run free
#define     PWM_DEBUG       DEBUG_STOP

// timer period (in CPU cycles)
#define     PWM_PERIOD      (CPU_FREQ/(SAMPLE_FREQ * SAMP_PRESCALE))

// prescaler for interrupt
#define     PWM_INT_PSCL    SAMP_PRESCALE

/**************************************************************
* Function for calling functions PWM_PWM_init and PWM_ADC_init;
* called from main()
* return: void
**************************************************************/
extern void PWM_init(void);

/**************************************************************
* Function for registers PWM1, PWM2, PWM3. Inside function an interrupt for ADC is
* enabled, period is set, compare register is set, tripzone register is set. PWM output
* signals are enabled.
* return:void
**************************************************************/
extern void PWM_update(float duty);

/**************************************************************
* Function for starting PWM1. Inside count mode can be set.
* return: void
**************************************************************/
extern void PWM_start(void);

/**************************************************************
* Function for setting period value in order to get desired period
* value. Cycle accurate.
* return: void
* arg1: desired period
**************************************************************/
extern void PWM_period(float perioda);

/**************************************************************
* Function for setting period in order to get desired
* frequency value. Cycle accurate.
* return: void
* arg1: desired frequency
**************************************************************/
extern void PWM_frequency(float frekvenca);

#endif  // end of __PWM_DRV_H__ definition

