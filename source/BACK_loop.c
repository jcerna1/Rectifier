/****************************************************************
* FILENAME:     BACK_loop.c
* DESCRIPTION:  background code
****************************************************************/

#include "BACK_loop.h"

// static local variables

// outside variables

// other functions

/**************************************************************
* Background function, running while NOT in interrupt
**************************************************************/
#pragma CODE_SECTION(BACK_loop, "ramfuncs");
void BACK_loop(void)
{
    while (1)
    {
        // infinite loop
        asm(" NOP");


    }   // end of while(1)
}       // end of BACK_loop
