
#ifndef SCANNING_H_
#define SCANNING_H_





#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"


#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

#define PING_FACTOR 930.24f //Factor to get distance in cm

volatile enum {RISE, FALL, DONE} pingState;
volatile uint32_t risingEdge;
volatile uint32_t fallingEdge;
volatile uint32_t pulseLength;
void TIMER3B_Handler(void);
void ping_init();
void send_pulse();
float get_pingDistance();



void adc_init();
uint16_t adc_read();



#endif /* SCANNING_H_ */
