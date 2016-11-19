/****************************************************************
 * FILENAME:     PER_int.c
 * DESCRIPTION:  periodic interrupt code
 ****************************************************************/
#include    "PER_int.h"
#include    "TIC_toc.h"
#include	"math.h"

//for ADC variables
int u_faza1 = 0;
int u_faza2 = 0;
int u_faza3 = 0;
int i_faza1 = 0;
int i_faza2 = 0;
int i_faza3 = 0;
int u_dc = 0;
int i_dc = 0;
int adc_pot1 = 0;
int adc_pot2 = 0;

// CPU load evaluation
float   cpu_load  = 0.0;
long    interrupt_cycles = 0;

// interrupt overflow (timeout) counter
int interrupt_overflow_counter = 0;

/**************************************************************
* variables for synchronization
**************************************************************/

/**************************************************************
 * Interrupt for regulation
 **************************************************************/
#pragma CODE_SECTION(PER_int, "ramfuncs");
void interrupt PER_int(void)
{
    // acknowledge interrupt:
    // Clear INT flag - ePWM1
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Clear INT flag- PIE
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // start timer
    interrupt_cycles = TIC_time;
    TIC_start();

    // calculate CPU load
    cpu_load = (float)interrupt_cycles / (CPU_FREQ/SWITCH_FREQ);

    // wait for the ADC to finish conversion
    ADC_wait();

    // INTERRUPT CODE HERE
    // INTERRUPT CODE HERE
    // INTERRUPT CODE HERE


	//za testiranje
	u_faza1 = U_FAZA1;
	u_faza2 = U_FAZA2;
	u_faza3 = U_FAZA3;
	i_faza1 = I_FAZA1;
	i_faza2 = I_FAZA2;
	i_faza3 = I_FAZA3;
	u_dc = U_DC;
	i_dc = I_DC;
	adc_pot1 = ADC_POT1;
	adc_pot2 = ADC_POT2;
	__asm ("      ESTOP0");

    // save values in buffer
    DLOG_GEN_update();

    /* check for interrupt while this interrupt is running -
     * if true, there is something wrong - if we count 10 such
     * occurances, something is seriousely wrong!
     */
    if (EPwm1Regs.ETFLG.bit.INT == TRUE)
    {
        // increase error counter
        interrupt_overflow_counter = interrupt_overflow_counter + 1;

        // if counter reaches 10, stop the operation
        if (interrupt_overflow_counter >= 10)
        {
            asm(" ESTOP0");
        }
    }

    // stop timer
    TIC_stop();

}   // end of PWM_int

/**************************************************************
 * Function for interrupt initialization
 **************************************************************/
void PER_int_setup(void)
{

    // Interrupt triggering
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // trigger on period
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;         // at each first case
    EPwm1Regs.ETCLR.bit.INT = 1;                // clear possible flag
    EPwm1Regs.ETSEL.bit.INTEN = 1;              // enable interrupt

    // Data logger initialization
    dlog.trig_value = 0;    			   // specify trigger value
    dlog.slope = Positive;                 // trigger on positive slope
    dlog.prescalar = 1;                    // store every  sample
    dlog.mode = Normal;                    // Normal trigger mode
    dlog.auto_time = 100;                  // number of calls to update function
    dlog.holdoff_time = 100;               // number of calls to update function

    //dlog.trig = &napetost;
    //dlog.iptr1 = &napetost;
    //dlog.iptr2 = &tok;


    // acknowledge interrupt
    EALLOW;
    PieVectTable.EPWM1_INT = &PER_int;
    EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    IER |= M_INT3;
    // interrupt in real time (for main loop and BACK_loop debugging)
    SetDBGIER(M_INT3);
}
