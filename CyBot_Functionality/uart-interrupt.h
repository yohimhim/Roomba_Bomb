#ifndef UART_H_
#define UART_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "uart-interrupt.h"
#include "stdbool.h"

extern volatile int cancel_scan_flag; // flag to tell the main program a scan was canceled
extern volatile int ping_scan_flag;        // flag to tell the main program a scan was called
extern volatile int ir_scan_flag;        // flag to tell the main program a scan was called
extern volatile int force_stop_flag;  // flag to tell the main program a forced stop was called
extern volatile int auto_flag;        // flag to tell the bot to switch to auto mode
extern volatile int manual_flag;      // flag to tell the bot to switch to manual mode

// UART1 device initialization for CyBot to PuTTY
void uart_interrupt_init(void);

// Send a byte over UART1 from CyBot to PuTTY
void uart_sendChar(char data);

// CyBot waits (i.e. blocks) to receive a byte from PuTTY
// returns byte that was received by UART1
// Not used with interrupts; see UART1_Handler
char uart_receive(void);

// Send a string over UART1
// Sends each char in the string one at a time
void uart_sendStr(const char *data);

// Interrupt handler for receive interrupts
void UART1_Handler(void);

#endif /* UART_H_ */
