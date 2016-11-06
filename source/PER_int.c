/****************************************************************
 * FILENAME:     PER_int.c
 * DESCRIPTION:  periodic interrupt code
 * AUTHOR:       Mitja Nemec
 * DATE:         16.1.2009
 *
 ****************************************************************/
#include    "PER_int.h"
#include    "TIC_toc.h"
#include	"math.h"

// za izracun napetosti
long napetost_raw = 0;
long napetost_offset = 2048;
float napetost_gain = 0.015749432; // =100/10100*4096/3.3*(1+50/12)
float napetost = 0.0;

// za izracun toka
long tok_raw = 0;
long tok_offset = 2048;
float tok_gain = 0.0;
float tok = 0.0;

// za oceno obremenjenosti CPU-ja
float   cpu_load  = 0.0;
long    interrupt_cycles = 0;

// interni stevec za 50 Hz
float   grid_ctr = 0; // grid counter
float   grid_prd = SAMPLE_FREQ/GRID_FREQ; // grid period: 400 = 20000/50


// spremenljivka s katero štejemo kolikokrat se je prekinitev predolgo izvajala
int interrupt_overflow_counter = 0;

/**************************************************************
* spremenljivke, ki jih potrebujemo za sinhronizacijo
**************************************************************/
float   PLL_out = SAMPLE_FREQ; // PLL_out = 20000
float	prejsnja_napetost = 0.0;     // spremenljivki za napetosti, ki ju bom potreboval
float	trenutna_napetost = 0.0;     // za ugotavljanje faznega zamika
float	fazni_zamik = 0.0; 	// fazni zamik: na zaèetku predpostavim, da je 0
float 	trenutna_napaka = 0.0;	// trenutna napaka PI regulatorja
float	prejsnja_napaka = 0.0;	// prejsnja napaka PI regulatorja
float	P = 0.0;	// proporcionalni èlen regulatorja
float	I = 0.0;	// integralni èlen regulatorja
float	Kp = 50.0;	// koeficient proporcionalnega èlena regulatorja
float	Ki = 5.0;	// koeficient integralnega èlena regulatorja
float	reg_out = 0.0; // izhodna vrednost PI regulatorja

/**************************************************************
 * Prekinitev, ki v kateri se izvaja regulacija
 **************************************************************/
#pragma CODE_SECTION(PER_int, "ramfuncs");
void interrupt PER_int(void)
{
    /* lokalne spremenljivke */
	int pozitivni_prehod = 0; // na zaèetku vsake prekinitve predvidevam, da ni pozitivnega prehoda èez nièlo

    // najprej povem da sem se odzzval na prekinitev
    // Spustimo INT zastavico casovnika ePWM1
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Spustimo INT zastavico v PIE enoti
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // pozenem stoprico
    interrupt_cycles = TIC_time;
    TIC_start();

    // izracunam obremenjenost procesorja
    cpu_load = (float)interrupt_cycles / (CPU_FREQ/SWITCH_FREQ);

    // pocakam da ADC konca s pretvorbo
    ADC_wait();

    // preracunam napetost
    napetost_raw = NAPETOST - napetost_offset;
    napetost = napetost_raw * napetost_gain;

    tok_raw = TOK - tok_offset;
    tok = tok_raw * tok_gain;

    /*******************************************************
    * Tukaj pride sinhronizacija - tukaj je potrebno doloèit frekvenco PWMja
    *******************************************************/
    prejsnja_napetost = trenutna_napetost; //trenutna napetost bom v naslednjem ciklu potreboval kot prejšnjo napetost, zato si jo shranim
    trenutna_napetost = napetost; //napetost, ki sem jo ravnokar dobil preko ADC, shranim kot trenutno
    
    if ((prejsnja_napetost<=0)&&(trenutna_napetost>=0)) { //èe je prejšnja napetost manjša ali enaka 0 ter je hkrati trenutna napetost veèja ali enaka 0, potem imamo pozitivni prehod skozi 0
    	pozitivni_prehod = 1;
    	fazni_zamik = ((grid_ctr-200)*PI)/200; //izraèunam fazni zamik, rezultat je v rangu od -PI do +PI
    }

    if (pozitivni_prehod) { //èe sem zaznal pozitivni prehod in izraèunal nov fazni zamik, grem v regulacijo
    	prejsnja_napaka = trenutna_napaka;  //shranim prejšnjo napako
    	trenutna_napaka = 0 - fazni_zamik;	//izraèunam trenutno napako

    	// PI regulacija
    	P  = trenutna_napaka;   //proporcionalni èlen: trenutna napaka
    	I += prejsnja_napaka;  //integralni èlen: vsota prejšnjih napak
    	reg_out = Kp*P + Ki*I; //izraèunam nov izhod PI regulatorja
    	if (reg_out<-4000) reg_out = -4000; //spodnja omejitev izhoda
    	if (reg_out>4000) reg_out = 4000;	//zgornja omejitev izhoda
    }

    //izraèunam novo frekvenco: osnovni frekvenci 20000Hz prištejem (pozitiven ali negativen) izhod iz regulatorja
    PLL_out = SAMPLE_FREQ+reg_out;
    
    // nastavim frekvenco
    PWM_frequency(PLL_out); //PLL_out je zeljena frekvenca PWM-ja v hertzih

    
    // interni stevec, ki gre od 0 do 400
    grid_ctr = grid_ctr + 1;
    if (grid_ctr >= grid_prd)
    {
        grid_ctr = 0; // ce pridemo do konca stevca, ga ponastavimo nazaj na 0
    }
    // generiram 50 Hz signal na izhodu
    if (grid_ctr >= (grid_prd/2))
    {
        PCB_out_on(); // ce je stevec manjsi od 200
    }
    else
    {
        PCB_out_off(); // ce je stevec vecji od 200
    }

    // spavim vrednosti v buffer za prikaz
    DLOG_GEN_update();

    /* preverim, èe me sluèajno èaka nova prekinitev.
       èe je temu tako, potem je nekaj hudo narobe
       saj je èas izvajanja prekinitve predolg
       vse skupaj se mora zgoditi najmanj 10krat,
       da reèemo da je to res problem
     */
    if (EPwm1Regs.ETFLG.bit.INT == TRUE)
    {
        // povecam stevec, ki steje take dogodke
        interrupt_overflow_counter = interrupt_overflow_counter + 1;

        // in ce se je vse skupaj zgodilo 10 krat se ustavim
        // v kolikor uC krmili kakšen resen HW, potem moèno
        // proporoèam lepše "hendlanje" takega dogodka
        // beri:ugasni moènostno stopnjo, ...
        if (interrupt_overflow_counter >= 10)
        {
            asm(" ESTOP0");
        }
    }

    // stopam
    TIC_stop();

}   // end of PWM_int

/**************************************************************
 * Funckija, ki pripravi vse potrebno za izvajanje
 * prekinitvene rutine
 **************************************************************/
void PER_int_setup(void)
{

    // Proženje prekinitve 
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    //sproži prekinitev na periodo
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;         //ob vsakem prvem dogodku
    EPwm1Regs.ETCLR.bit.INT = 1;                //clear possible flag
    EPwm1Regs.ETSEL.bit.INTEN = 1;              //enable interrupt

    // inicializiram data logger
    dlog.trig_value = 0;    // specify trigger value
    dlog.slope = Positive;                 // trigger on positive slope
    dlog.prescalar = 1;                    // store every  sample
    dlog.mode = Normal;                    // Normal trigger mode
    dlog.auto_time = 100;                  // number of calls to update function
    dlog.holdoff_time = 100;               // number of calls to update function

    dlog.trig = &napetost;
    dlog.iptr1 = &napetost;
    dlog.iptr2 = &tok;


    // registriram prekinitveno rutino
    EALLOW;
    PieVectTable.EPWM1_INT = &PER_int;
    EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    IER |= M_INT3;
    // da mi prekinitev teèe  tudi v real time naèinu
    // (za razhoršèevanje main zanke in BACK_loop zanke)
    SetDBGIER(M_INT3);
}
