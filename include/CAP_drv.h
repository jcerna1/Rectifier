/**************************************************************
* FILE:         CAP_drv.c
* DESCRIPTION:  Capture unit driver
* AUTHOR:       Mitja Nemec
* START DATE:   19.1.2012
* VERSION:      1.0
*
* CHANGES :
* VERSION   DATE        WHO                 DETAIL
* 1.0       19.1.2012   Mitja Nemec         Initial version
*
****************************************************************/
#ifndef __CAP_DRV_H__
#define __CAP_DRV_H__

#include    "DSP28x_Project.h"
#include    "define.h"

// katero enoto uporabljamo
#define     CAP_MODUL		ECap2Regs

// katera je najnizja prekvenca preden vrne napako
#define     CAP_FREQ_MIN    18

// delovanje modula ob debug-dogodkih
// (0-stop at zero, 1-stop immediately, 2-run free)
#define     CAP_DEBUG		2   

/**************************************************************
* funkcija ki vrne periodo signala na vhodu cap enote
*
**************************************************************/
extern float CAP_period(void);

/**************************************************************
* inicializacija CAP-ja
*
**************************************************************/
extern void CAP_init(void);


#endif /* __CAP_DRV_H__ */
