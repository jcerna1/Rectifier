/**************************************************************
* FILE:         PCB_util.c 
* DESCRIPTION:  PCB initialization & Support Functions
**************************************************************/
#include "PCB_util.h"

/**************************************************************
* LED 1 turn ON
**************************************************************/
void PCB_LED_on(void)
{
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
}

/**************************************************************
* LED 1 turn OFF
**************************************************************/
void PCB_LED_off(void)
{
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
}

/**************************************************************
* LED 1 toggle
**************************************************************/
void PCB_LED_toggle(void)
{
	GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
}


/**************************************************************
* OUTPUT ON
**************************************************************/
void PCB_out_on(void)
{
	GpioDataRegs.GPADAT.bit.GPIO0 = 1;
}

/**************************************************************
* OUTPUT OFF
**************************************************************/
void PCB_out_off(void)
{
	GpioDataRegs.GPADAT.bit.GPIO0 = 0;
}

/**************************************************************
* PCB initialization
**************************************************************/
void PCB_init(void)
{
    EALLOW;

    /* OUTPUTS */
    // LED on Piccolo Board
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO34 = 0;

    // consumer
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    GpioDataRegs.GPADAT.bit.GPIO0 = 0;

    EDIS;

}
