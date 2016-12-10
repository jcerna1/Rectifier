/************************************************************** 
* FILE:         PWM_drv.c
* DESCRIPTION:  A/D in PWM driver for TMS320F2808
****************************************************************/
#include "PWM_drv.h"



/**************************************************************
* Function for registers PWM1, PWM2, PWM3. Inside function an interrupt for ADC is
* enabled, period is set, compare register is set, tripzone register is set. PWM output
* signals are enabled.
* return:void
**************************************************************/
void PWM_init(void)
{
//EPWM Module 4
    // setup timer base 
    EPwm4Regs.TBPRD = PWM_PERIOD/2;       // set to 25us, PWM_PERIOD = 50us
    EPwm4Regs.TBCTL.bit.PHSDIR = 0;       // count up after sync
    EPwm4Regs.TBCTL.bit.CLKDIV = 0;
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0;
    EPwm4Regs.TBCTL.bit.SYNCOSEL = 1;     // sync out on zero
    EPwm4Regs.TBCTL.bit.PRDLD = 0;        // shadowed period reload
    EPwm4Regs.TBCTL.bit.PHSEN = 0;        // master timer does not sync
    EPwm4Regs.TBCTR = 1;

        // debug mode behaviour
    #if PWM_DEBUG == 0
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 0;  // stop immediately
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 0;  // stop immediately
    #endif
    #if PWM_DEBUG == 1
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 1;  // stop when finished
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 1;  // stop when finished
    #endif
    #if FB_DEBUG == 2
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 3;  // run free
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 3;  // run free
    #endif
    
    // Compare registers
    EPwm4Regs.CMPA.half.CMPA = PWM_PERIOD/4;                 //50% duty cycle

    // Init Action Qualifier Output A Register 
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;  // clear output on CMPA_UP
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;    // set output on CMPA_DOWN

    // Dead Time
    
    // Trip zone 

    // Event trigger
    // Proženje ADC-ja
    EPwm4Regs.ETSEL.bit.SOCASEL = 2;    // trigger on period
    EPwm4Regs.ETPS.bit.SOCAPRD = 1;     // at each first case
    EPwm4Regs.ETCLR.bit.SOCA = 1;       // clear possible flag
    EPwm4Regs.ETSEL.bit.SOCAEN = 1;     // enable ADC Start Of conversion
    // Proženje prekinitve 
    EPwm4Regs.ETSEL.bit.INTSEL = 2;             // trigger on period
    EPwm4Regs.ETPS.bit.INTPRD = PWM_INT_PSCL;   // at each first case
    EPwm4Regs.ETCLR.bit.INT = 1;                // clear possible flag
    EPwm4Regs.ETSEL.bit.INTEN = 1;              // enable interrupt

//EPWM Module 2

//EPWM Module 3
 
// output pin setup

}   //end of PWM_PWM_init

/**************************************************************
* Function for choosing correct transistor combination, depending
* on duty cycle and chosen vector.
* return: void
* arg1: duty cycle from 0.0 to 1.0 (format IQ)
**************************************************************/
void PWM_update(float duty)
{
   int compare;

    // duty cycle protection
    if (duty < 0.0) duty = 0.0;
    if (duty > 1.0) duty = 1.0;

    // calculate value of compare register (from period and duty cycle)
    compare = (int)((PWM_PERIOD/2) * duty);

    // write the value to register
    EPwm4Regs.CMPA.half.CMPA = compare;
    

}  //end of AP_PWM_update

/**************************************************************
* Function for setting period value in order to get desired period
* value. Cycle accurate.
* return: void
* arg1: desired period
**************************************************************/
void PWM_period(float perioda)
{
    // variables
    float   temp_tbper;
    static float ostanek = 0;
    long celi_del;

    // calculate TBPER (CPU_FREQ * period)
    temp_tbper = perioda * CPU_FREQ/2;
    
    // separate integer and remainder
    celi_del = (long)temp_tbper;
    ostanek = temp_tbper - celi_del;
    // increase integer, in case remainder is bigger than 1
    if (ostanek > 1.0)
    {
        ostanek = ostanek - 1.0;
        celi_del = celi_del + 1;
    }
    
    // set TBPER
    EPwm4Regs.TBPRD = celi_del;
}   //end of FB_period

/**************************************************************
* Function for setting period in order to get desired
* frequency value. Cycle accurate.
* return: void
* arg1: desired frequency
**************************************************************/
void PWM_frequency(float frekvenca)
{
    // variables
    float   temp_tbper;
    static float ostanek = 0;
    long celi_del;

    // calculate TBPER (CPU_FREQ / SAMPLING_FREQ) - 1
    temp_tbper = (CPU_FREQ/2) / frekvenca;

    // separate integer and remainder
    celi_del = (long)temp_tbper;
    ostanek = temp_tbper - celi_del;
    // increase integer, in case remainder is bigger than 1
    if (ostanek > 1.0)
    {
        ostanek = ostanek - 1.0;
        celi_del = celi_del + 1;
    }
    
    // set TBPER
    EPwm4Regs.TBPRD = celi_del - 1;
}   //end of FB_frequency
  
/**************************************************************
* Function for starting PWM1. Inside count mode can be set.
* return: void
**************************************************************/
void PWM_start(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
    
}   //end of AP_PWM_start



