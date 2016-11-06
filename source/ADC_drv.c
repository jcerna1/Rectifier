/**************************************************************
* FILE:         ADC_drv.c
* DESCRIPTION:  A/D driver for piccolo devices
****************************************************************/
#include "ADC_drv.h"

/**************************************************************
* initialize ADC
**************************************************************/
void ADC_init(void)
{
    // *IMPORTANT*
    // The Device_cal function, which copies the ADC calibration values from TI reserved
    // OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
    // Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC to function according
    // to specification. The clocks to the ADC MUST be enabled before calling this
    // function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    EDIS;

    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
    // after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the DSP2802x_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD = 1;       // Power ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN = 1;        // Power ADC - enable analogue circuit inside MCU
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // Select interal BG (internal reference)
    AdcRegs.ADCCTL1.bit.TEMPCONV = 1;       // Internal temperature sensor on ADCIN5 input
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    EDIS;

    DELAY_US(1000L);         // Delay before converting ADC channels

    // Configure ADC
    EALLOW;
    
    // ADC trigger setup
    ADC_MODUL1.CMPB = 0x0000;
    ADC_MODUL1.ETSEL.bit.SOCASEL = ET_CTR_ZERO;   // trigger on period
    ADC_MODUL1.ETPS.bit.SOCAPRD = ET_1ST;        // at each first case
    ADC_MODUL1.ETCLR.bit.SOCA = 1;               // clear possible flag
    ADC_MODUL1.ETSEL.bit.SOCAEN = 1;             // enable ADC Start Of conversion

    // SOC0 config
    AdcRegs.ADCSOC0CTL.bit.CHSEL = 0x6;     //set SOC0 channel select to ADCINA1
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 0x05;  //set SOC0 to start trigger on EPWM1A
    AdcRegs.ADCSOC0CTL.bit.ACQPS = 0X015;

    // SOC1 config
    AdcRegs.ADCSOC1CTL.bit.CHSEL = 0x6;     //set SOC1 channel select to ADCINA1
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 0x05;  //set SOC1 to start trigger on EPWM1A
    AdcRegs.ADCSOC1CTL.bit.ACQPS = 0X015;

    // SOC2 config
    AdcRegs.ADCSOC2CTL.bit.CHSEL = 0x4;     //set SOC2 channel select to ADCINA0
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL = 0x05;  //set SOC2 to start trigger on EPWM1A
    AdcRegs.ADCSOC2CTL.bit.ACQPS = 0X015;

    // set interrupt flag when last coversion is finished ...
    // while interrupt is still disabled, the interrupt flag will serve us
    // to detect the end of coversion
    AdcRegs.INTSEL1N2.bit.INT1SEL = 0x00;   // interrupt1 tiggers signal EOC1 (this one is the last to run)
    AdcRegs.INTSEL1N2.bit.INT1E = 1;        // interrupt at interrupt event is disabled (so flag can be set)
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;   // clear the flag

    EDIS;

}   //end of AP_ADC_init

/**************************************************************
* This function is for waiting for ADC to finish conversion ...
* return: void
**************************************************************/
void ADC_wait(void)
{
    while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0)
    {
        /* DO NOTHING */
    }
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;  // clear ADC flag bit
}
