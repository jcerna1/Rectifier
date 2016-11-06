/*
 * File name:
 *               GPIO.c
 * Project name: 
 *               PicolloControlStick
 * Description:
 *               GPIO configuration
 *               Based on TI file F2806x_Gpio.c
 * Author:
 *               Jurij Cernatic
 * Version:
 *               1.7
 * Date:
 *               08/09/2016
 */

#include "define.h"

#include "DSP28x_Project.h"

#include "GPIO.h"



/*
 * Perform the GPIO initialization.
 *
 *    All pins are set as GPIO, as inputs and without pull ups.
 *
 *    Outputs signals are configured
 *
 *    Inputs signals are configured
 *
 *    Outputs default state is configured with APIs
 */
void GPIO_Init(void)
{
	EALLOW;

	// Global pin configuration

		// Each GPIO pin can be:
		// a) a GPIO input/output
		// b) peripheral function 1
		// c) peripheral function 2
		// d) peripheral function 3
		// By default, all are GPIO Inputs
		GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
		GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
		GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO47
		GpioCtrlRegs.GPBMUX2.all = 0x0000;	   // GPIO functionality GPIO48-GPIO63
		GpioCtrlRegs.AIOMUX1.all = 0x0000;     // Dig.IO funct. applies to AIO2,4,6,10,12,14

		GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are GP inputs
		GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs
		GpioCtrlRegs.AIODIR.all = 0x0000;      // AIO2,4,6,10,12,14 are digital inputs

		// Each input can have different qualification
		// a) input synchronized to SYSCLKOUT
		// b) input qualified by a sampling window
		// c) input sent asynchronously (valid for peripheral inputs only)
		GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT
		GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
		GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO47 Synch to SYSCLKOUT
		GpioCtrlRegs.GPBQSEL2.all = 0x0000;	   // GPIO48-GPIO63 Synch to SYSCLKOUT

		// Pull-ups can be enabled or disabled.
		//GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
		//GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO44
		GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
		GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO44


	// Outputs pin configuration

		//PH_L1
		GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO1 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO1 = 1; // output mode

		//PH_L2
		GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO3 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO3 = 1; // output mode

		//PH_L3
		GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO5 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO5 = 1; // output mode

		//PH_H1
		GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO0 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; // output mode

		//PH_H2
		GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO2 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; // output mode

		//PH_H3
		GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO4 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO4 = 1; // output mode

		//LED_R
		GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO12 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO12 = 1; // output mode

		//LED_G
		GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO17 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO17 = 1; // output mode

		//LED_Y
		GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1; // Disable pullup
		GpioDataRegs.GPASET.bit.GPIO16 = 1; // Load output latch
		GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0; //Selected as a GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO16 = 1; // output mode7


	// Input configuration

		// Sampling time configuration: 30 * SYSCLKOUT periods = 30 * (1/90MHz) = 1/3 µs
		// 3 Samples to qualify
		// Note: if GPxCTRL[QUALPRDn] != 0, sampling period = 2 × GPxCTRL[QUALPRDn] × SYSCLKOUT periods

		// Specifies the sampling period for pins GPIO0 to GPIO7
		GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 15;
		// Specifies the sampling period for pins GPIO8 to GPIO15
		GpioCtrlRegs.GPBCTRL.bit.QUALPRD1 = 15;
		// Specifies the sampling period for pins GPIO16 to GPIO23
		GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 15;
		// Specifies the sampling period for pins GPIO24 to GPIO31
		GpioCtrlRegs.GPBCTRL.bit.QUALPRD3 = 15;

		// SW1
		GpioCtrlRegs.GPBPUD.bit.GPIO34 = 1;   // Disable pull up
		GpioCtrlRegs.GPBQSEL1.bit.GPIO34 = 1; // Qualification using == GPIO_HIGH) 3 samples => 1 µs to detect front

		// SW2
		GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;   // Disable pull up
		GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 1; // Qualification using 3 samples => 1 µs to detect front



	//Signal associated output pin default state

		GPIO_Clear(GPIO_PH_L1);
		GPIO_Clear(GPIO_PH_L2);
		GPIO_Clear(GPIO_PH_L3);
		GPIO_Clear(GPIO_PH_H1);
		GPIO_Clear(GPIO_PH_H2);
		GPIO_Clear(GPIO_PH_H3);
		GPIO_Clear(GPIO_LED_R);
		GPIO_Clear(GPIO_LED_G);
		GPIO_Clear(GPIO_LED_Y);

	EDIS;
}

/*
 * Return the state of SW1. Can be GPIO_HIGH or GPIO_LOW
 */
 Uint8 GPIO_u8GetSW1(void)
{
	if(GpioDataRegs.GPBDAT.bit.GPIO34 == GPIO_HIGH)
	{
		return GPIO_HIGH;
	}
	return GPIO_LOW;
}

 /*
  * Return the state of SW2. Can be GPIO_HIGH or GPIO_LOW
  */
 Uint8 GPIO_u8GetSW2(void)
{
	if(GpioDataRegs.GPADAT.bit.GPIO18 == GPIO_HIGH)
	{
		return GPIO_HIGH;
	}
	return GPIO_LOW;
}
