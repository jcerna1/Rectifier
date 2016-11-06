/**************************************************************
* FILE:         ADC_drv.c
* DESCRIPTION:  A/D driver for piccolo devices
****************************************************************/
#ifndef     __ADC_DRV_H__
#define     __ADC_DRV_H__

#include    "DSP28x_Project.h"

// set which PWM module triggers ADC
#define     ADC_MODUL1      EPwm1Regs

// location of results
#define     ADC_RESULT0     (AdcResult.ADCRESULT0)
#define     NAPETOST        (AdcResult.ADCRESULT1)
#define     TOK             (AdcResult.ADCRESULT2)

/**************************************************************
* initialize ADC
**************************************************************/
extern void ADC_init(void);

/**************************************************************
* This function is for waiting for ADC to finish conversion ...
* return: void
**************************************************************/
extern void ADC_wait(void);

#endif /* __ADC_DRV_H__ */
