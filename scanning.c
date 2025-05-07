
#include "scanning.h"
#include "lcd.h"
#include "math.h"
#include "String.h"


void ping_init(void){
    //Turn on clock to TIMER3
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    //Port B (bit1) system clock
    SYSCTL_RCGCGPIO_R |= 0x02;

    //Pin 3 init
    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

    //Configure the timer for input capture mode
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

    //Edge select
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

    //Set to a 16 bit counter
    TIMER3_CFG_R |= TIMER_CFG_16_BIT;

    //Capture mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;

    TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;
    TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    //105 and 142 interrupt #
    //Timer 3B: 36 Register 5, set Enable (EN1) BIT4 or 0x10
    NVIC_EN1_R |= 0x10;

    //register TIMER3B interrupt handler
    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    //initialize global interrupts
    IntMasterEnable();
}


void send_pulse(){
    //alternative and port control (AFSEL and CTL)
    //alt function off
    GPIO_PORTB_AFSEL_R &= ~(0x08);

    //off
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);

    // set PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;

    // set PB3 to high
    GPIO_PORTB_DATA_R |= 0x08;

    // wait at least 5 microseconds based on data sheet
    timer_waitMicros(5);

    // set PB3 to low
    GPIO_PORTB_DATA_R &= 0xF7;
    pingState = RISE;

    GPIO_PORTB_DIR_R &= 0xF7;
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x00007000;

    //enable TIMER3B and start counting
    TIMER3_CTL_R |= TIMER_CTL_TBEN;// 0x100
}


void TIMER3B_Handler(void)
{
    if(TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
        if(pingState == RISE)
        {
            //clear capture interrupt flag
            TIMER3_ICR_R = TIMER_ICR_CBECINT;

            //captures time of rising edge event
            risingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            pingState = FALL;
        }
        else if(pingState == FALL)
        {
            //clear capture interrupt flag
            TIMER3_ICR_R = TIMER_ICR_CBECINT;

            //capture time of falling edge
            fallingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            pingState = DONE;
            TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
        }
    }
}


float get_pingDistance()
{
    send_pulse();
    while(pingState != DONE)
    {

    }

    int overflow = (fallingEdge < risingEdge);

    pulseLength = fallingEdge + (overflow<<24) - risingEdge;

    return pulseLength / PING_FACTOR;
}

//////////////////////////////////////////////////////////////////////////////////////////////////



void ir_init(){
    SYSCTL_RCGCGPIO_R |= 2;
    SYSCTL_RCGCADC_R |= 1;
    GPIO_PORTB_AFSEL_R |= 16;
    GPIO_PORTB_DEN_R &= ~16;
    GPIO_PORTB_AMSEL_R |= 16;
    GPIO_PORTB_ADCCTL_R = 0;

    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;
    //enable raw interrupt status
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
    //ADC0_PSSI_R = ADC_PSSI_SS1;
}


unsigned ir_read(){
    //initiate SS1 conversion
    ADC0_PSSI_R=ADC_PSSI_SS0;
    //wait for ADC conversion to be complete
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){
        //wait
        }
    //grab result
    ADC0_ISC_R=ADC_ISC_IN0;
    int value = ADC0_SSFIFO0_R;
    return value;
}

double get_irDistance() {
    int i;
    double average = 0;

    for(i = 0; i < 50; i++){
        average += (double) ir_read();
        timer_waitMillis(1);
    }

    average = average / 50.0;
    return average;
}


