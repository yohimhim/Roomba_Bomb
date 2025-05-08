
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






void adc_init(){
    SYSCTL_RCGCGPIO_R |= 0x2; //activate clock for Port B
    while(!(SYSCTL_RCGCGPIO_R & 0x2));

    GPIO_PORTB_AFSEL_R |= 0x10; //enable alternate function
    GPIO_PORTB_DEN_R &= ~(0x10); // disable digital I/O
    GPIO_PORTB_AMSEL_R |= 0x10; // enable analog functionality
    GPIO_PORTB_DIR_R &= ~(0x10); // make input

    SYSCTL_RCGCADC_R |= 0x1; // activate ADC
    while(!(SYSCTL_RCGCADC_R & 0x1));
    ADC0_ACTSS_R &= ~(0x8); //disable sample sequencer
    while(ADC0_ACTSS_R & 0x8);
    ADC0_EMUX_R &= ~(0xF000);
    ADC0_SSMUX3_R |= 0xA;
    ADC0_IM_R &= ~(0x8);
    ADC0_SSCTL3_R |= 0x4;
    ADC0_SAC_R |= 0x6;
    ADC0_ACTSS_R |= 0x8; // enable sample sequencer
}

uint16_t adc_read(){
    uint16_t temp;

    ADC0_PSSI_R |= 0x8; // 1) initiate SS3
    while(!(ADC0_RIS_R & 0x8));
    temp = ADC0_SSFIFO3_R & 0xFFF; // 3) read temp
    ADC0_ISC_R |= 0x8;
    return temp;
}
