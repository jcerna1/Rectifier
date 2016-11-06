/****************************************************************
* FILENAME:     PER_int.h
* DESCRIPTION:  periodic interrupt header file
****************************************************************/
#ifndef     __PER_INT_H__
#define     __PER_INT_H__

#include    "DSP28x_Project.h"

#include    "IQmathLib.h"
#include    "define.h"
#include    "globals.h"

#include    "PWM_drv.h"
#include    "ADC_drv.h"
#include    "PCB_util.h"
#include    "math.h"

#include    "DLOG_gen.h"


/**************************************************************
* Function for interrupt initialization
**************************************************************/
extern void PER_int_setup(void);

#endif // end of __PER_INT_H__ definition
