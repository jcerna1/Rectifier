/*
 * File name:
 *               GPIO.h
 * Project name: 
 *               PicolloControlStick
 * Description:
 *               GPIO header file
 *
 *               
 * Author:
 *               Jurij Cernatic
 * Version:
 *               1.6
 * Date:
 *               08/09/2016
 */

/*
 * Signals list
 *
 *    All pins are on port A of µC
 *       No need of port B considerations. GPIOA and GPIOB methods are simplified to GPIO.
 *
 *         controlStick
 * Name    Pin number    GPIO port  GPIO number Direction
 * PH1_LI  21            A          1           Output
 * PH2_LI  23            A          3           Output
 * PH3_LI  15            A          5           Output
 * PH1_HI  17            A          0           Output
 * PH2_HI  27            A          2           Output
 * PH3_HI  19            A          4           Output
 * LED_R    8            A          12          Output
 * LED_G   20            A          17          Output
 * LED_Y   24            A          16          Output
 * SW1     30            B          34          Input
 * SW2     29            A          18          Input
 */

#ifndef GPIO_H
#define GPIO_H

#include "DSP28x_Project.h"

//Component configuration

// All signals shall be associated to their µc GPIO
#define GPIO_PH_L1            ((Uint32) 1 << 1) //
#define GPIO_PH_L1            ((Uint32) 1 << 1) // GPIO1
#define GPIO_PH_L2            ((Uint32) 1 << 3) // GPIO3
#define GPIO_PH_L3            ((Uint32) 1 << 5) // GPIO5
#define GPIO_PH_H1            ((Uint32) 1 << 0) // GPIO0
#define GPIO_PH_H2            ((Uint32) 1 << 2) // GPIO2
#define GPIO_PH_H3            ((Uint32) 1 << 4) // GPIO4
#define GPIO_LED_R            ((Uint32) 1 << 12) // GPIO12
#define GPIO_LED_G            ((Uint32) 1 << 17) // GPIO17
#define GPIO_LED_Y            ((Uint32) 1 << 16) // GPIO16
#define GPIO_SW1              ((Uint32) 1 << 2) // GPIO34
#define GPIO_SW2              ((Uint32) 1 << 18) // GPIO18


//External definitions

// Represent an input in low state
#define GPIO_LOW              ((Uint8) 0u)
// Represent an input in high state
#define GPIO_HIGH             ((Uint8) 1u)


extern void GPIO_Init(void);

//Used to set the signalName GPIO output to high state in one operation. No stack operations due to function call.
extern inline void GPIO_Set(Uint32 u32SignalMask)
{
	GpioDataRegs.GPASET.all |= u32SignalMask;
}

// Used to clear the signalName GPIO output to low state in one operation. No stack operations due to function call.
extern inline void GPIO_Clear(Uint32 u32SignalMask)
{
	GpioDataRegs.GPACLEAR.all |= u32SignalMask;
}

// Used to toggle the signalName GPIO output state in one operation. No stack operations due to function call.
inline void GPIO_Toggle(Uint32 u32SignalMask)
{
	GpioDataRegs.GPATOGGLE.all |= u32SignalMask;
}

extern Uint8 GPIO_u8GetSW1(void);
extern Uint8 GPIO_u8GetSW2(void);

#endif
