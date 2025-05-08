

#include "servo.h"
#include "Timer.h"
#include <stdint.h>
#include "lcd.h"

 int leftVal = 311150;
 int rightVal = 283361;

int new_position = 90;
bool direction_clockwise = false; //false is counterclockwise
bool moved = false;
int x = 0;

void servoo_init (void){
    SYSCTL_RCGCGPIO_R |= 0b10;  // enable clock on PORT B of GPIO
    SYSCTL_RCGCTIMER_R |= 0b10; // enable clock for timer 1
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};    // wait for ready on PORT B
    GPIO_PORTB_DIR_R |= 0b100000;    // set PB5 to output
    GPIO_PORTB_DEN_R |= 0b100000;    // enable digital output on PB5

    GPIO_PORTB_AFSEL_R |= 0b100000; // enable alt function on PB5
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;  // set alt function as T3CCP1 function
    GPIO_PORTB_PCTL_R |= 0x700000;  // ... on PB3 (set bits [15..12] to 7) // correct to PB5

    //--------- TIMER SETUP ----------
    TIMER1_CTL_R &= ~0x100; // disable Timer during startup
    TIMER1_CFG_R = 0x4;     // using 16bit with prescaler
    TIMER1_TBMR_R |= 0b1010;
    TIMER1_TBMR_R &= ~0b0101; // configure Timer B Mode for PWM one-shot
    TIMER1_CTL_R &= ~0x4000; //noniverted

    TIMER1_TBILR_R = 0xE200; //320000 // Loads the starting count value into timer (max value bc
                             // we are counting down)
    TIMER1_TBPR_R = 0x4;      // Loads the starting count value into the remaining 8bit prescaler
                             // (max because we're counting down) (0x04e200)
    TIMER1_TBMATCHR_R = 0xC000; // 20ms low pulse width
    TIMER1_TBPMR_R = 0x4;

    TIMER1_CTL_R |= 0x100; // re-enable Timer B
}


void servoo_move(uint16_t degrees){
    // w(d) = (1/180)d+1 (in ms)
    // cycles(w in ms) = 16000000*(w in ms/1000)
//    float highPulse_ms = (1.0/180.0)*degrees+1.0;
//    uint32_t widthCycles = 16000000.0 * (((1.0/180.0)*degrees+1.0)/1000.0);
//    uint32_t lowCycles = 320000 - widthCycles;

    //unsigned int lowCycles = 320000 - ((320000-288000)/180)*degrees;
    unsigned int lowCycles = leftVal - ((leftVal-rightVal)/180)*degrees;


    TIMER1_TBMATCHR_R = (lowCycles & 0xFFFF);
    TIMER1_TBPMR_R = lowCycles>>16;
}
