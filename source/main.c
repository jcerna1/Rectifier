/****************************************************************
* FILENAME:     main.c
* DESCRIPTION:  initialization code
****************************************************************/
#include "main.h"
#include "DSP28x_Project.h"
#include "GPIO.h"
#include "TEST_GPIO.h"

/**************************************************************
* Initialization function
**************************************************************/
void main(void)
{
    // PLL & clock
    InitSysCtrl();
    
    // GPIO initialization
    // old function for initialization: InitGpio();
    GPIO_Init();

    // test function for GPIO
    // TEST_AllGPIOs();

    // generic init of PIE
    InitPieCtrl();

    // basic vector table
    InitPieVectTable();

    // eCAP initialization
    CAP_init();

    // ADC & PWM initialization
    ADC_init();
    PWM_init();

    // periodic interrupt initialization
    PER_int_setup();

    // predem omogocimprekinitve pockam 100 ms, da se sistem ustali
    DELAY_US(100000L);

    // enable interrupts
    EINT;
    ERTM;

    // force PWM 1,2,3 outputs to LOW
    PWM_forceLOW();

    // start timer for ADC & interrupt triggering
    PWM_start();

    // go to infinite loop in background
    BACK_loop();
}   // end of main
