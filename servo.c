#include "servo.h"
#include "Timer.h"
#include "math.h"
#include "lcd.h"
#include "button.h"

void servo_INIT(void){
    //Enable clocks to GPIO and Timer
    SYSCTL_RCGCGPIO_R |= 0x2;
    SYSCTL_RCGCTIMER_R |= 0x2;

    while((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    while((SYSCTL_PRTIMER_R & 0x02) == 0) {};

    //Configure Port B3
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;

    //Configure timer 1B
    TIMER1_CTL_R &= ~0x100; //disable
    TIMER1_CFG_R |= 0x4;
    TIMER1_TBMR_R |= 0xA;

    TIMER1_TBILR_R = 320000 & 0xFFFF; // start value
    TIMER1_TBPR_R = 320000 >> 16;
    TIMER1_TBPMR_R &= 0x0;
    TIMER1_TBMATCHR_R &= 0x0; //match value

    TIMER1_CTL_R |= 0x100;       //enable
}
