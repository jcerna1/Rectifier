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
#define     U_FAZA1     (AdcResult.ADCRESULT0)
#define     U_FAZA2     (AdcResult.ADCRESULT1)
#define     U_FAZA3     (AdcResult.ADCRESULT2)
#define		I_FAZA1		(AdcResult.ADCRESULT4)
#define		I_FAZA2		(AdcResult.ADCRESULT5)
#define		I_FAZA3		(AdcResult.ADCRESULT6)
#define		U_DC		(AdcResult.ADCRESULT8)
#define		I_DC		(AdcResult.ADCRESULT9)
#define		ADC_POT1	(AdcResult.ADCRESULT12)
#define		ADC_POT2	(AdcResult.ADCRESULT14)

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
