/************************************************************** 
* FILE:         PWM_drv.c
* DESCRIPTION:  A/D in PWM driver for TMS320F2808
****************************************************************/
#include "PWM_drv.h"



/**************************************************************
* Function for registers PWM1, PWM2, PWM3, PWM4. Inside function an interrupt for ADC is
* enabled, period is set, compare register is set, tripzone register is set. PWM output
* signals are enabled.
* return:void
**************************************************************/
//Ttbclk = 0.000000125
void PWM_init(void)
{
//EPWM Module 1
	EPwm1Regs.TBPRD = PWM_PHASE_PERIOD/4; // set frequency (Tpwm=2*TBPRD*Ttbclk)
	EPwm1Regs.CMPA.half.CMPA = PWM_PHASE_PERIOD/8;
	EPwm1Regs.CMPB = PWM_PHASE_PERIOD/8;
	EPwm1Regs.TBPHS.half.TBPHS = 0; // set phase register to 0
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // symmetrical up-down mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE; // master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // sync down-stream module
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM1A: incrementing - set
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR; // decrementing - clear
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR; // set actions for EPWM1B: incrementing - clear
	EPwm1Regs.AQCTLB.bit.CBD = AQ_SET; // decrementing - set
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable dead-band module
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
	EPwm1Regs.DBFED = 20; // FED = 20 TBCLKs
	EPwm1Regs.DBRED = 20; // RED = 20 TBCLKs


//EPWM Module 2
	EPwm2Regs.TBPRD = PWM_PHASE_PERIOD/4; // set frequency (Tpwm=2*TBPRD*Ttbclk)
	EPwm2Regs.CMPA.half.CMPA = PWM_PHASE_PERIOD/8;
	EPwm2Regs.CMPB = PWM_PHASE_PERIOD/8;
	EPwm2Regs.TBPHS.half.TBPHS = (PWM_PHASE_PERIOD/2)/3; // phase delay by 120 deg
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // symmetrical up-down mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE; // slave module
	EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN; // count DOWN on sync (= 120 deg)
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM2A: incrementing - set
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR; // decrementing - clear
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR; // set actions for EPWM2B: incrementing - clear
	EPwm2Regs.AQCTLB.bit.CBD = AQ_SET; // decrementing - set
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable dead-band module
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
	EPwm2Regs.DBFED = 20; // FED = 20 TBCLKs
	EPwm2Regs.DBRED = 20; // RED = 20 TBCLKs


//EPWM Module 3
	EPwm3Regs.TBPRD = PWM_PHASE_PERIOD/4; // set frequency (Tpwm=2*TBPRD*Ttbclk)
	EPwm3Regs.TBPHS.half.TBPHS = (PWM_PHASE_PERIOD/2)/3;; // phase delay by 240 deg
	EPwm3Regs.CMPA.half.CMPA = PWM_PHASE_PERIOD/8;
	EPwm3Regs.CMPB = PWM_PHASE_PERIOD/8;
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // symmetrical up-down mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE; // slave module
	EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP; // count UP on sync (= 120 deg)
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // send sync signal to PWM4 when CTR=0
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM3A: incrementing - set
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR; // decrementing - clear
	EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR; // set actions for EPWM3B: incrementing - clear
	EPwm3Regs.AQCTLB.bit.CBD = AQ_SET; // decrementing - set
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable dead-band module
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
	EPwm3Regs.DBFED = 20; // FED = 20 TBCLKs
	EPwm3Regs.DBRED = 20; // RED = 20 TBCLKs


//EPWM Module 4
    // setup timer base 
    EPwm4Regs.TBPRD = PWM_PERIOD/2;       // set frequency (Tpwm=2*TBPRD*Ttbclk)
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
 
}   //end of PWM_init

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
* Function for starting PWM4. Inside count mode can be set.
* return: void
**************************************************************/
void PWM_start(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
    
}   //end of AP_PWM_start



