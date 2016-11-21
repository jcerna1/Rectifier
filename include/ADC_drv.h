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
#define     U_FAZA1     (AdcResult.ADCRESULT1) //voltage of phase 1
#define     U_FAZA2     (AdcResult.ADCRESULT2) //voltage of phase 2
#define     U_FAZA3     (AdcResult.ADCRESULT3) //voltage of phase 3
#define		I_FAZA1		(AdcResult.ADCRESULT4) //current of phase 1
#define		I_FAZA2		(AdcResult.ADCRESULT5) //current of phase 2
#define		I_FAZA3		(AdcResult.ADCRESULT6) //current of phase 3
#define		U_DC		(AdcResult.ADCRESULT7) //voltage of DC output
#define		I_DC		(AdcResult.ADCRESULT8) //current of DC output
#define		ADC_POT1	(AdcResult.ADCRESULT9) //voltage of potentiometer 1
#define		ADC_POT2	(AdcResult.ADCRESULT10) //voltage of potentiometer 2

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
