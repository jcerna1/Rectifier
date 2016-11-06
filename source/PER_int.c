/****************************************************************
 * FILENAME:     PER_int.c
 * DESCRIPTION:  periodic interrupt code
 ****************************************************************/
#include    "PER_int.h"
#include    "TIC_toc.h"
#include	"math.h"

// voltage calculation
long napetost_raw = 0;
long napetost_offset = 2048;
float napetost_gain = 0.015749432; // =100/10100*4096/3.3*(1+50/12)
float napetost = 0.0;

// current calculation
long tok_raw = 0;
long tok_offset = 2048;
float tok_gain = 0.0;
float tok = 0.0;

// CPU load evaluation
float   cpu_load  = 0.0;
long    interrupt_cycles = 0;

// internal 50 Hz counter
float   grid_ctr = 0; // grid counter
float   grid_prd = SAMPLE_FREQ/GRID_FREQ; // grid period: 400 = 20000/50


// interrupt overflow (timeout) counter
int interrupt_overflow_counter = 0;

/**************************************************************
* variables for synchronization
**************************************************************/
float   PLL_out = SAMPLE_FREQ; // PLL_out = 20000
float	prejsnja_napetost = 0.0;     // previous voltage
float	trenutna_napetost = 0.0;     // current voltage
float	fazni_zamik = 0.0; 	// phase shift (delay)
float 	trenutna_napaka = 0.0;	// previous PI regulator error
float	prejsnja_napaka = 0.0;	// current PI regulator error
float	P = 0.0;	// proportional value of PI regulator
float	I = 0.0;	// integral value of PI regulator
float	Kp = 50.0;	// proportional coefficient
float	Ki = 5.0;	// integral coefficient
float	reg_out = 0.0; // PI regulator output

/**************************************************************
 * Interrupt for regulation
 **************************************************************/
#pragma CODE_SECTION(PER_int, "ramfuncs");
void interrupt PER_int(void)
{
    /* local variables */
	int pozitivni_prehod = 0; // assuming there is no positive edge of the signal

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

    // calculate voltage and current
    napetost_raw = NAPETOST - napetost_offset;
    napetost = napetost_raw * napetost_gain;

    tok_raw = TOK - tok_offset;
    tok = tok_raw * tok_gain;

    /*******************************************************
    * PWM frequency calculation (synchronization)
    *******************************************************/
    prejsnja_napetost = trenutna_napetost; // save current voltage as previous, for the next cycle
    trenutna_napetost = napetost; // save current voltage in new variable
    
    if ((prejsnja_napetost<=0)&&(trenutna_napetost>=0)) { // detect positive edge of the signal
    	pozitivni_prehod = 1;
    	fazni_zamik = ((grid_ctr-200)*PI)/200; // calculate phase shift (range -PI .. PI)
    }

    if (pozitivni_prehod) { // in case of positive edge, start regulation
    	prejsnja_napaka = trenutna_napaka;  // save previous error
    	trenutna_napaka = 0 - fazni_zamik;	// calculate previous error

    	// PI regulation
    	P  = trenutna_napaka;   // proportional value: current error
    	I += prejsnja_napaka;  // integral value: sum of previous errors
    	reg_out = Kp*P + Ki*I; // calculate new PI regulator output
    	if (reg_out<-4000) reg_out = -4000; // lower limit of the output
    	if (reg_out>4000) reg_out = 4000;	// upper limit of the output
    }

    // calculate new frequency: base frequency of 20000Hz + (positive or negative) regulator output
    PLL_out = SAMPLE_FREQ+reg_out;
    
    // set the new frequency (Hz)
    PWM_frequency(PLL_out);

    
    // internal counter from 0 to 400
    grid_ctr = grid_ctr + 1;
    if (grid_ctr >= grid_prd)
    {
        grid_ctr = 0; // reset counter if 400
    }
    // generate 50 Hz signal on the output
    if (grid_ctr >= (grid_prd/2))
    {
        PCB_out_on(); // if counter lower than 200
    }
    else
    {
        PCB_out_off(); // if counter higher than 200
    }

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

    dlog.trig = &napetost;
    dlog.iptr1 = &napetost;
    dlog.iptr2 = &tok;


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
