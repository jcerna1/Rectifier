/****************************************************************
* FILENAME:     define.h           
* DESCRIPTION:  file with global define macros
****************************************************************/
#ifndef     __DEFINE_H__
#define     __DEFINE_H__

#include    "IQmathLib.h"

// PWM frequency
#define     SWITCH_FREQ     20000

// prescaler
#define   	SAMP_PRESCALE   1

// sample frequency
#define   	SAMPLE_FREQ     (SWITCH_FREQ/SAMP_PRESCALE)

// sample period
#define     SAMPLE_TIME     (1.0/SAMPLE_FREQ)

// electrical power grid frequency (Hz)
#define		GRID_FREQ       50

// CPU frequency (Hz)
#define     CPU_FREQ        80000000

// mathematical constants
#define		SQRT3			1.7320508075688772935274463415059
#define		SQRT2			1.4142135623730950488016887242097
#define		PI				3.1415926535897932384626433832795

// boolean values
typedef enum {FALSE = 0, TRUE} bool;

// peripherals on DEBUG_STOP:
// 0 stop immediately, 1 stop when finished, 2 run free
#define     DEBUG_STOP      0

#endif // end of __DEFINE_H__ definition
