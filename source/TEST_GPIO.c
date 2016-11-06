/*
 * File name:
 *               TEST_GPIO.c
 * Project name: 
 *               PicolloControlStick
 * Description:
 *               Function test of GPIO drivers
 * Author:
 *               Jurij Cernatic
 * Version:
 *               1.5
 * Date:
 *               06/09/2016
 */


#include "define.h"

#include "DSP28x_Project.h"

#include "GPIO.h"
#include "TEST_GPIO.h"

void TEST_AllGPIOs(void)
{
	TEST_PH_L1();
	TEST_PH_L2();
	TEST_PH_L3();
	TEST_PH_H1();
	TEST_PH_H2();
	TEST_PH_H3();
	TEST_LED_R();
	TEST_LED_G();
	TEST_LED_Y();
	TEST_SW1();
	TEST_SW2();
}

void TEST_PH_L1(void)
{
	TEST_UC_HALT //verify that PH_L1 is low (default state), then run the program

	GPIO_Set(GPIO_PH_L1);
	TEST_UC_HALT //verify that PH_L1 is high, then run the program

	GPIO_Clear(GPIO_PH_L1);
	TEST_UC_HALT //verify that PH_L1 is low, then run the program

	GPIO_Toggle(GPIO_PH_L1);
	TEST_UC_HALT //verify that PH_L1 is high, then run the program

	GPIO_Toggle(GPIO_PH_L1);
	TEST_UC_HALT //verify that PH_L1 is low, then run the program
}

void TEST_PH_L2(void)
{
	TEST_UC_HALT //verify that PH_L2 is low (default state), then run the program

	GPIO_Set(GPIO_PH_L2);
	TEST_UC_HALT //verify that PH_L2 is high, then run the program

	GPIO_Clear(GPIO_PH_L2);
	TEST_UC_HALT //verify that PH_L2 is low, then run the program

	GPIO_Toggle(GPIO_PH_L2);
	TEST_UC_HALT //verify that PH_L2 is high, then run the program

	GPIO_Toggle(GPIO_PH_L2);
	TEST_UC_HALT //verify that PH_L2 is low, then run the program
}

void TEST_PH_L3(void)
{
	TEST_UC_HALT //verify that PH_L3 is low (default state), then run the program

	GPIO_Set(GPIO_PH_L3);
	TEST_UC_HALT //verify that PH_L3 is high, then run the program

	GPIO_Clear(GPIO_PH_L3);
	TEST_UC_HALT //verify that PH_L3 is low, then run the program

	GPIO_Toggle(GPIO_PH_L3);
	TEST_UC_HALT //verify that PH_L3 is high, then run the program

	GPIO_Toggle(GPIO_PH_L3);
	TEST_UC_HALT //verify that PH_L3 is low, then run the program
}

void TEST_PH_H1(void)
{
	TEST_UC_HALT //verify that PH_H1 is low (default state), then run the program

	GPIO_Set(GPIO_PH_H1);
	TEST_UC_HALT //verify that PH_H1 is high, then run the program

	GPIO_Clear(GPIO_PH_H1);
	TEST_UC_HALT //verify that PH_H1 is low, then run the program

	GPIO_Toggle(GPIO_PH_H1);
	TEST_UC_HALT //verify that PH_H1 is high, then run the program

	GPIO_Toggle(GPIO_PH_H1);
	TEST_UC_HALT //verify that PH_H1 is low, then run the program
}

void TEST_PH_H2(void)
{
	TEST_UC_HALT //verify that PH_H2 is low (default state), then run the program

	GPIO_Set(GPIO_PH_H2);
	TEST_UC_HALT //verify that PH_H2 is high, then run the program

	GPIO_Clear(GPIO_PH_H2);
	TEST_UC_HALT //verify that PH_H2 is low, then run the program

	GPIO_Toggle(GPIO_PH_H2);
	TEST_UC_HALT //verify that PH_H2 is high, then run the program

	GPIO_Toggle(GPIO_PH_H2);
	TEST_UC_HALT //verify that PH_H2 is low, then run the program
}

void TEST_PH_H3(void)
{
	TEST_UC_HALT //verify that PH_H3 is low (default state), then run the program

	GPIO_Set(GPIO_PH_H3);
	TEST_UC_HALT //verify that PH_H3 is high, then run the program

	GPIO_Clear(GPIO_PH_H3);
	TEST_UC_HALT //verify that PH_H3 is low, then run the program

	GPIO_Toggle(GPIO_PH_H3);
	TEST_UC_HALT //verify that PH_H3 is high, then run the program

	GPIO_Toggle(GPIO_PH_H3);
	TEST_UC_HALT //verify that PH_H3 is low, then run the program
}

void TEST_LED_R(void)
{
  TEST_UC_HALT //verify that LED_R is low (default state), then run the program

	GPIO_Set(GPIO_LED_R);
	TEST_UC_HALT //verify that LED_R is high, then run the program

	GPIO_Clear(GPIO_LED_R);
	TEST_UC_HALT //verify that LED_R is low, then run the program

	GPIO_Toggle(GPIO_LED_R);
	TEST_UC_HALT //verify that LED_R is high, then run the program

	GPIO_Toggle(GPIO_LED_R);
	TEST_UC_HALT //verify that LED_R is low, then run the program
}

void TEST_LED_G(void)
{
	TEST_UC_HALT //verify that LED_G is low (default state), then run the program

	GPIO_Set(GPIO_LED_G);
	TEST_UC_HALT //verify that LED_G is high, then run the program

	GPIO_Clear(GPIO_LED_G);
	TEST_UC_HALT //verify that LED_G is low, then run the program

	GPIO_Toggle(GPIO_LED_G);
	TEST_UC_HALT //verify that LED_G is high, then run the program

	GPIO_Toggle(GPIO_LED_G);
	TEST_UC_HALT //verify that LED_G is low, then run the program
}

void TEST_LED_Y(void)
{
	TEST_UC_HALT //verify that LED_Y is low (default state), then run the program

	GPIO_Set(GPIO_LED_Y);
	TEST_UC_HALT //verify that LED_Y is high, then run the program

	GPIO_Clear(GPIO_LED_Y);
	TEST_UC_HALT //verify that LED_Y is low, then run the program

	GPIO_Toggle(GPIO_LED_Y);
	TEST_UC_HALT //verify that LED_Y is high, then run the program

	GPIO_Toggle(GPIO_LED_Y);
	TEST_UC_HALT //verify that PH3_LI is low, then run the program
}

void TEST_SW1(void)
{
	Uint8 u8Sw1Value;

	TEST_UC_HALT //Apply GND on SW1, then run the program

	u8Sw1Value = GPIO_u8GetSW1();

	if(u8Sw1Value != GPIO_LOW)
	{
		TEST_UC_HALT //SW1 is not low, test failure
	}
	else
	{
		TEST_UC_HALT//SW1 is low, continue
	}

	TEST_UC_HALT //Apply VCC on SW1, then run the program

	u8Sw1Value = GPIO_u8GetSW1();

	if(u8Sw1Value != GPIO_HIGH)
	{
		TEST_UC_HALT //SW1 is not high, test failure
	}
	else
	{
		TEST_UC_HALT//SW1 is high, continue
	}
}

void TEST_SW2(void)
{
	Uint8 u8SW2Value;

	TEST_UC_HALT //Apply GND on SW2, then run the program

	u8SW2Value = GPIO_u8GetSW2();

	if(u8SW2Value != GPIO_LOW)
	{
		TEST_UC_HALT //SW2 is not low, test failure
	}
	else
	{
		//SW2 is low, continue
	}

	TEST_UC_HALT //Apply VCC on SW2, then run the program

	u8SW2Value = GPIO_u8GetSW2();

	if(u8SW2Value != GPIO_HIGH)
	{
		TEST_UC_HALT //SW2 is not high, test failure
	}
	else
	{
		//SW2 is high, continue
	}
}


void TEST_BRIDGE(void)
{
	TEST_UC_HALT
	/*
	WARNING: LIMIT CURRENT BEFORE USING THIS TEST!
	Tests MOSFET bridge by shorting each phase individually. Low side MOSFET is switched ON first,
	followed by HIGH side MOSFET.
	*/
	//phase 1
	GPIO_Set(GPIO_PH_L1);
	GPIO_Set(GPIO_PH_H1); // phase 1 will be shorted after running this line!
	TEST_UC_HALT
	GPIO_Clear(GPIO_PH_L1); // phase 1 will be open again
	GPIO_Clear(GPIO_PH_H1);
	//phase 2
	GPIO_Set(GPIO_PH_L2);
	GPIO_Set(GPIO_PH_H2); // phase 2 will be shorted after running this line!
	TEST_UC_HALT
	GPIO_Clear(GPIO_PH_L2); // phase 2 will be open again
	GPIO_Clear(GPIO_PH_H2);
	//phase 3
	GPIO_Set(GPIO_PH_L3);
	GPIO_Set(GPIO_PH_H3); // phase 3 will be shorted after running this line!
	TEST_UC_HALT
	GPIO_Clear(GPIO_PH_L3); // phase 3 will be open again
	GPIO_Clear(GPIO_PH_H3);
}
