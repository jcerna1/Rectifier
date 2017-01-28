/**************************************************************
* FILE:         CAP_drv.c
* DESCRIPTION:  Capture unit driver
* AUTHOR:       Mitja Nemec
* START DATE:   19.1.2012
* VERSION:      1.0
*
* CHANGES :
* VERSION   DATE        WHO                 DETAIL
* 1.0       19.1.2012  Mitja Nemec         Initial version
*
****************************************************************/
#include "CAP_drv.h"


/**************************************************************
* inicializacija zunanjega ADC-ja (ADS8556)
*
**************************************************************/
void CAP_init(void)
{
    // input pin cofiguration
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 3; //configure GPIO7 pin as eCAP2
    GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0x10; //sampling period = 20xTsysclkout
    GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 2; //qualification using 6 samples
    EDIS;

	CAP_MODUL.ECCTL1.bit.CAP1POL = 0;
	CAP_MODUL.ECCTL1.bit.CAP2POL = 0;
	CAP_MODUL.ECCTL1.bit.CAP3POL = 0;
	CAP_MODUL.ECCTL1.bit.CAP4POL = 0;
	CAP_MODUL.ECCTL1.bit.CTRRST1 = 1;
	CAP_MODUL.ECCTL1.bit.CTRRST2 = 1;
	CAP_MODUL.ECCTL1.bit.CTRRST3 = 1;
	CAP_MODUL.ECCTL1.bit.CTRRST4 = 1;
	CAP_MODUL.ECCTL1.bit.PRESCALE = 0;
	CAP_MODUL.ECCTL1.bit.CAPLDEN = 1;

	#if CAP_DEBUG == 0
    CAP_MODUL.ECCTL1.bit.FREE_SOFT = 1;  // stop after current cycle
    #endif
    #if CAP_DEBUG == 1
    CAP_MODUL.ECCTL1.bit.FREE_SOFT = 0;  // stop after current cycle
    #endif
    #if CAP_DEBUG == 2
    CAP_MODUL.ECCTL1.bit.FREE_SOFT = 3;  // run free
    #endif
	
	CAP_MODUL.ECCTL2.bit.STOP_WRAP = 0;
	CAP_MODUL.ECCTL2.bit.CAP_APWM = 0;
	CAP_MODUL.ECCTL2.bit.CONT_ONESHT = 0;
	CAP_MODUL.ECCTL2.bit.SYNCO_SEL = 2;
	CAP_MODUL.ECCTL2.bit.SYNCI_EN = 0;
	CAP_MODUL.ECCTL2.bit.TSCTRSTOP = 1;
}

/**************************************************************
* funkcija ki vrne periodo signala na vhodu cap enote
*
**************************************************************/
float CAP_period(void)
{
	unsigned long cycle_number;
    float   perioda;

    cycle_number = CAP_MODUL.CAP1;

    if (CAP_MODUL.ECFLG.bit.CTROVF == 0)
    {
        if (CAP_MODUL.TSCTR > (CPU_FREQ/CAP_FREQ_MIN))
        {
            perioda = 0.0;
        }
        else
        {
            perioda = cycle_number * (1.0/CPU_FREQ);
        }
        
    }
    else
    {
        perioda = 0.0;
        CAP_MODUL.ECCLR.bit.CTROVF = 1;
    }
    
    return (perioda);
}

