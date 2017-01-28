/****************************************************************
 * FILENAME:     PER_int.c
 * DESCRIPTION:  periodic interrupt code
 ****************************************************************/
#include    "PER_int.h"
#include    "TIC_toc.h"
#include	"math.h"
#include 	"GPIO.h"
#include 	"define.h"

//ADC variables
int F1 = 0;
int F2 = 0;
int F3 = 0;
int DC = 0;
float u_faza1 = 0.0; //phase 1 voltage [V]
float u_faza2 = 0.0; //phase 2 voltage [V]
float u_faza3 = 0.0; //phase 3 voltage [V]
float i_faza1 = 0.0; //phase 1 current [A]
float i_faza2 = 0.0; //phase 2 current [A]
float i_faza3 = 0.0; //phase 3 current [A]
float u_dc = 0.0; //DC output voltage [V]
float i_dc = 0.0; //DC output current [A]
float adc_pot1 = 0.0; //potentiometer 1 position [0.0-1.0]
float adc_pot2 = 0.0; //potentiometer 2 position [0.0-1.0]

//eCAP period
float eCAP_period = 0.0;

//GAIN/OFFSET values
float u_faza1_gain = 0.004474398;
float u_faza1_offset = 0.012685033;
float u_faza2_gain = 0.004417942;
float u_faza2_offset = 0.024390625;
float u_faza3_gain = 0.004421897;
float u_faza3_offset = 0.022075010;
float u_dc_gain = 0.004429593;
float u_dc_offset = 0.014512117;

//PLL/PI variables
float PI_out = 0.0; //PI regulator output frequency [Hz]
float previous_voltage = 0.0; //phase 1 voltage from previous interrupt
float current_voltage = 0.0; //current phase 1 voltage
float phase_shift = 0.0; //phase shift value
float previous_error = 0.0; //PI regulator error from previous interrupt
float current_error = 0.0; //current PI regulator error
float P = 0.0; //proportional term of PI regulator
float I = 0.0; //integral term of I regulator
float Kp = 1000.0; //proportional coefficient value
float Ki = 100.0; //integral coefficient value
float sample_ctr = 0; //sample counter

// CPU load evaluation
float   cpu_load  = 0.0;
long    interrupt_cycles = 0;

// interrupt overflow (timeout) counter
int interrupt_overflow_counter = 0;

/**************************************************************
 * Interrupt for regulation
 **************************************************************/
#pragma CODE_SECTION(PER_int, "ramfuncs");
void interrupt PER_int(void)
{
	/* local variables */
	int positive_edge = 0; //assume there is no positive edge at the beginning of the interrupt

    // acknowledge interrupt:
    // Clear INT flag - ePWM4
    EPwm4Regs.ETCLR.bit.INT = 1;
    // Clear INT flag- PIE
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // start timer
    interrupt_cycles = TIC_time;
    TIC_start();

    // calculate CPU load
    cpu_load = (float)interrupt_cycles / (CPU_FREQ/SWITCH_FREQ);

    // wait for the ADC to finish conversion
    ADC_wait();

    /*****************************************************************
    * Calculation of voltages and currents from raw ADC data
    *****************************************************************/

    F1 = U_FAZA1;
    F2 = U_FAZA2;
    F3 = U_FAZA3;
    DC = U_DC;
	u_faza1 = U_FAZA1*u_faza1_gain+u_faza1_offset;
	u_faza2 = U_FAZA2*u_faza2_gain+u_faza2_offset;
	u_faza3 = U_FAZA3*u_faza3_gain+u_faza3_offset;
	i_faza1 = I_FAZA1;
	i_faza2 = I_FAZA2;
	i_faza3 = I_FAZA3;
	u_dc = U_DC*u_dc_gain+u_dc_offset;
	i_dc = I_DC;
	adc_pot1 = ADC_POT1/4095.0;
	adc_pot2 = ADC_POT2/4095.0;

	/*****************************************************************
	* Calculation of a new PWM4 (interrupt) frequency
	*****************************************************************/
	previous_voltage = current_voltage; //save current voltage as previous voltage for next iteration
	current_voltage = u_faza1; //set phase 1 voltage as current voltage

	if ((previous_voltage < 7.5) && (current_voltage >= 7.5)) { //detect positive edge
		positive_edge = 1; //positive edge detected
		phase_shift = (((sample_ctr-(SAMPLES/2))*PI))/(SAMPLES/2); //calculate phase shift
		//range from -PI to +PI
	}

	if (positive_edge) { //if positive edge was detected, start regulation algorithm
		previous_error = current_error; //save current error
		current_error = 0 - phase_shift; //calculate current error
		//PI REGULATION
		P = current_error; //proportional term: current error
		I += previous_error; //integral term: sum of previous errors
		PI_out = Kp*P + Ki*I; //PI regulator output frequency
		if (PI_out > 4000) PI_out = 4000; //upper limit of PI output
		if (PI_out < -4000) PI_out = -4000; //lower limit of PI output
	}

	//set new PWM4 (interrupt) frequency in Hz
	//PWM_frequency((24000+PI_out));
	PWM_frequency(14000+PI_out);
	//TEST_UC_HALT;

	//internal sample counter
	sample_ctr = sample_ctr + 1; //increment current sample number by 1
	if (sample_ctr == SAMPLES) {
		sample_ctr = 0; //reset sample counter when it reaches SAMPLES value
	}

	//find eCAP period value
	eCAP_period = CAP_period(); //eCAP period in seconds
	TEST_UC_HALT;

    // save values in buffer
    DLOG_GEN_update();

    /* check for interrupt while this interrupt is running -
     * if true, there is something wrong - if we count 10 such
     * occurances, something is seriousely wrong!
     */
    if (EPwm4Regs.ETFLG.bit.INT == TRUE)
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
    EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // trigger on period
    EPwm4Regs.ETPS.bit.INTPRD = ET_1ST;         // at each first case
    EPwm4Regs.ETCLR.bit.INT = 1;                // clear possible flag
    EPwm4Regs.ETSEL.bit.INTEN = 1;              // enable interrupt

    // Data logger initialization
    dlog.trig_value = 20;    			   // specify trigger value
    dlog.slope = Negative;                 // trigger on positive slope
    dlog.prescalar = 1;                    // store every  sample
    dlog.mode = Normal;                    // Normal trigger mode
    dlog.auto_time = 100;                  // number of calls to update function
    dlog.holdoff_time = 100;               // number of calls to update function

    dlog.trig = &sample_ctr;
    dlog.iptr1 = &sample_ctr;
    dlog.iptr2 = &u_faza1;


    // acknowledge interrupt
    EALLOW;
    PieVectTable.EPWM4_INT = &PER_int;
    EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
    IER |= M_INT3;
    // interrupt in real time (for main loop and BACK_loop debugging)
    SetDBGIER(M_INT3);
}
