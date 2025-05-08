#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart-interrupt.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "uart-interrupt.h"
#include "stdbool.h"

volatile int ping_scan_flag = 0;         // start scan
volatile int ir_scan_flag = 0;         // start scan
volatile int cancel_scan_flag = 0;  // cancel scan
volatile int force_stop_flag = 0;   // stop all scanners and movement
volatile int manual_flag = 0;       // toggle to manual mode
volatile int auto_flag = 0;         // toggle to autonomous mode


void uart_interrupt_init(void){
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0x02;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0x02;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
  while ((SYSCTL_PRUART_R & 0x02) == 0) {};

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x03;

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0x03;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R = 0x11;

  //calculate baud rate
  uint16_t iBRD = 0x8; //use equations
  uint16_t fBRD = 0x2C; //use equations

  //turn off UART1 while setting it up
  UART1_CTL_R &= 0x0;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0x60;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //////Enable interrupts

  //first clear RX interrupt flag (clear by writing 1 to ICR)
  UART1_ICR_R |= 0b00010000;

  //enable RX raw interrupts in interrupt mask register
  UART1_IM_R |= 0x10;

  //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

  //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
  NVIC_EN0_R |= 0x40;

  //tell CPU to use ISR handler for UART1 (see interrupt.h file)
  //from system header file: #define INT_UART1 22
  IntRegister(INT_UART1, UART1_Handler);

  //globally allow CPU to service interrupts (see interrupt.h file)
  IntMasterEnable();

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R = 0b1100000001;

}

void uart_sendChar(char data){
    while(UART1_FR_R & 0x20){};
    UART1_DR_R = data;
}

char uart_receive(void){
    char data= 0;
    while(UART1_FR_R & UART_FR_RXFE){};

    data = (char)(UART1_DR_R & 0xFF);
    return data;
}

void uart_sendStr(const char *data){
    int i =0;
    while(data[i] != '\0'){
        uart_sendChar(data[i]);
        i++;
    }
}

void UART1_Handler(void)
{
    char byte_received;

    //check if handler called due to RX event
    if (UART1_MIS_R & 0x10) //checking if bit field 4 RXMIS is a 1 (true)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0b00010000;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        byte_received = (UART1_DR_R & 0xFF);
        uart_sendChar(byte_received);

        if (byte_received){
            //if byte received is a carriage return
            if (byte_received == 'z'){
                ping_scan_flag = 1;
            } else if (byte_received == '1'){
                auto_flag = 1;
            } else if (byte_received == '2'){
                manual_flag = 1;
            }
        }
    }
}
