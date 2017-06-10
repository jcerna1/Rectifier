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
float DCpower = 0.0; //power of (rectified) DC output [W]
float ACpower = 0.0; //AC power before rectification [W]
float neutral = 0.0; //instantaneous neutral voltage
float alpha = 0.000157067; //alpha value for low pass filter

//synchronization variables
long CMPA = 0;
long CMPB = 0;
int valid_signal = 0;
int eCAP_call = 0;
float eCAP_period = 0.0;
int synchronization = 0;
int phase_samples = 1000; //number of samples to check for a phase shift
int samples_counter = 0;
double shift_treshold = 0.2;
float counter = 100000000000;

//GAIN/OFFSET values
float u_faza1_gain = 0.004474398;
float u_faza1_offset = 0.012685033;
float u_faza2_gain = 0.004417942;
float u_faza2_offset = 0.024390625;
float u_faza3_gain = 0.004421897;
float u_faza3_offset = 0.022075010;
float i_faza1_gain = 0.100363945;
float i_faza1_offset = -202.474223129;
float i_faza2_gain = 0.097303207;
float i_faza2_offset = -197.038994169;
float i_faza3_gain = 0.097599535;
float i_faza3_offset = -197.463380009;
float u_dc_gain = 0.004429593;
float u_dc_offset = 0.014512117;
float i_dc_gain = 0.095410094;
float i_dc_offset = -25.284518524;

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

// za opazovanje stevcev
float	pwm_timer1 = 0;
float	pwm_timer2 = 0;
float	pwm_timer3 = 0;

// interrupt overflow (timeout) counter
int interrupt_overflow_counter = 0;

/**************************************************************
 * Interrupt for regulation
 **************************************************************/
#pragma CODE_SECTION(PER_int, "ramfuncs")
void interrupt PER_int(void)
{
	/* local variables */

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
	u_faza1 = U_FAZA1*u_faza1_gain+u_faza1_offset;
	u_faza2 = U_FAZA2*u_faza2_gain+u_faza2_offset;
	u_faza3 = U_FAZA3*u_faza3_gain+u_faza3_offset;
	i_faza1 = I_FAZA1*i_faza1_gain+i_faza1_offset;
	i_faza2 = I_FAZA2*i_faza2_gain+i_faza2_offset;
	i_faza3 = I_FAZA3*i_faza3_gain+i_faza3_offset;
	u_dc = U_DC*u_dc_gain+u_dc_offset;
	i_dc = I_DC*i_dc_gain+i_dc_offset;

	/*****************************************************************
	* Calculation of AC and DC power
	*****************************************************************/
	neutral = (u_faza1+u_faza2+u_faza3)/3.0; //calculation of instantaneous neutral voltage
	ACpower = (alpha*(((u_faza1-neutral)*i_faza1)+((u_faza2-neutral)*i_faza2)+((u_faza3-neutral)*i_faza3)) + ((1-alpha)*ACpower)); //AC POWER
	DCpower = (alpha*(u_dc*i_dc)) + ((1-alpha)*DCpower); //DC POWER
	/*****************************************************************
	 * Calculation of potentiometer relative position
	 *****************************************************************/
	adc_pot1 = ADC_POT1/4095.0; //potentiometer 1 for duty cycle setting
	adc_pot2 = ADC_POT2/4095.0; //potentiometer 2 for phase setting
	if (adc_pot1 > 0.66) { //duty cycle capping
		adc_pot1 = 0.66;
	}

	/*****************************************************************
	* Asynchronous software-forced triggering of the MOSFETs
	*****************************************************************/
	CMPA = EPwm1Regs.CMPA.half.CMPA;
	CMPB = EPwm1Regs.CMPB;
	if (synchronization == 1) {
		if (EPwm1Regs.TBCTR < CMPA) { //if PWM1 counter less than CMPA
			EPwm1Regs.AQCSFRC.bit.CSFA = 1; //turn EPWM1A OFF
		}
		else {
			EPwm1Regs.AQCSFRC.bit.CSFA = 2; //turn EPWM1A ON
		}
		if (EPwm1Regs.TBCTR > CMPB) { //if PWM1 counter more than CMPB
			EPwm1Regs.AQCSFRC.bit.CSFB = 1; //turn EPWM1B OFF
		}
		else {
			EPwm1Regs.AQCSFRC.bit.CSFB = 2; //turn EPWM1B ON
		}

		if (EPwm2Regs.TBCTR < CMPA) { // ... same principle for PWM2 ...
			EPwm2Regs.AQCSFRC.bit.CSFA = 1;
		}
		else {
			EPwm2Regs.AQCSFRC.bit.CSFA = 2;
		}
		if (EPwm2Regs.TBCTR > CMPB) {
			EPwm2Regs.AQCSFRC.bit.CSFB = 1;
		}
		else {
			EPwm2Regs.AQCSFRC.bit.CSFB = 2;
		}

		if (EPwm3Regs.TBCTR < CMPA) { // ... and PWM3
			EPwm3Regs.AQCSFRC.bit.CSFA = 1;
		}
		else {
			EPwm3Regs.AQCSFRC.bit.CSFA = 2;
		}
		if (EPwm3Regs.TBCTR > CMPB) {
			EPwm3Regs.AQCSFRC.bit.CSFB = 1;
		}
		else {
			EPwm3Regs.AQCSFRC.bit.CSFB = 2;
		}
	}
	/*else {
		EPwm1Regs.AQCSFRC.bit.CSFB = 1; //EPWM1B = OFF
		EPwm2Regs.AQCSFRC.bit.CSFB = 1; //EPWM2B = OFF
		EPwm3Regs.AQCSFRC.bit.CSFB = 1; //EPWM3B = OFF
	}*/

	/*CMPB1 = EPwm1Regs.CMPB;
	CMPB2 = EPwm2Regs.CMPB;
	if ((EPwm1Regs.TBSTS.bit.CTRDIR == 1)) {
		EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_CMPB;
	}
	else {
		EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	}*/
	/*if ((EPwm2Regs.TBSTS.bit.CTRDIR == 1) && (EPwm2Regs.TBCTR > CMPB2)) {
		EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_CMPB;
	}
	else {
		EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	}*/
	//asm(" ESTOP0");

	/*****************************************************************
	* Checking for a valid signal & determining initial frequency
	*****************************************************************/

	eCAP_period = CAP_period(); //eCAP period in seconds
	eCAP_period = ((1/eCAP_period)*SAMPLES); //calculation of PWM4 frequency
	//asm(" ESTOP0");
	if ((eCAP_period > (13000)) && (eCAP_period < (240000))) { //if PH1 frequency is in range 310Hz-1000Hz
		valid_signal = 1;
	}
	else {
		valid_signal = 0;
	}

	/*****************************************************************
	* Calculation of a new PWM4 (interrupt) frequency
	*****************************************************************/

	previous_voltage = current_voltage; //save current voltage as previous voltage for next iteration
	current_voltage = u_faza1; //set phase 1 voltage as current voltage

	if ((valid_signal == 1) && (previous_voltage < 7.5) && (current_voltage >= 7.5)) { //detect positive edge
		if (synchronization == 1) {
			EPwm1Regs.TBCTL.bit.SWFSYNC = 1;
		}
		phase_shift = (((sample_ctr-(SAMPLES/2))*PI))/(SAMPLES/2); //calculate phase shift
		//range from -PI to +PI
		previous_error = current_error; //save current error
		current_error = 0 - phase_shift; //calculate current error
		//PI REGULATION
		P = current_error; //proportional term: current error
		I += previous_error; //integral term: sum of previous errors
		PI_out = Kp*P + Ki*I; //PI regulator output frequency
		if (PI_out > 16000) PI_out = 16000; //upper limit of PI output
		if (PI_out < -16000) PI_out = -16000; //lower limit of PI output
		PWM_frequency(eCAP_period+PI_out); //PWM4 frequency = 80x phase frequency (PWM123)
		//PWM_frequency(62400+PI_out);
		//PWM_duty(adc_pot1);
		PWM_duty(adc_pot1);
		//PWM_phase(adc_pot2);
		PWM_phase(0.5);
	}

	/*****************************************************************
	* Continuous synchronization check
	*****************************************************************/
	if (fabs(phase_shift) < shift_treshold) { //compare current phase shift with treshold
		if (samples_counter < phase_samples) { //
			samples_counter = samples_counter + 1;
		}
		else {
			synchronization = 1;
			GPIO_Set(GPIO_LED_G);
		}
	}
	else { //if current phase shift is higher than treshold, sync is lost or not reached yet
		synchronization = 0;
		samples_counter = 0;
		GPIO_Clear(GPIO_LED_G);
	}

	/*if (synchronization == 1) { //if synchronization successful
		//asm(" ESTOP0");
	}*/



	//internal sample counter
	sample_ctr = sample_ctr + 1; //increment current sample number by 1
	if (sample_ctr == SAMPLES) {
		sample_ctr = 0; //reset sample counter when it reaches SAMPLES value
	}

    // save values in buffer
	//pwm_timer1 = EPwm1Regs.TBCTR;
	//pwm_timer2 = EPwm2Regs.TBCTR;
	//pwm_timer3 = EPwm3Regs.TBCTR;

    //DLOG_GEN_update();

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
    dlog.iptr2 = &pwm_timer1;
    dlog.iptr3 = &pwm_timer2;
    dlog.iptr4 = &pwm_timer3;


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
