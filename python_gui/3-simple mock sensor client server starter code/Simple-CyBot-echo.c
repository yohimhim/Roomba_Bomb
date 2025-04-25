///////////////////////////////////////////////////////////////////////
// Name: Simple-CyBot-echo.c                                         //
// Author: Phillip Jones (phjones@iastate.edu)                       //
// Date: 09/26/2023                                                  //
// Description: Simple Cybot program that is an Echo'ish program for //
//              communicating with the Starter Client Socket code.   //
///////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "movement.h"

int main (void) {

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data);

  timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
                  // and enables time functions (e.g. timer_waitMillis)
  lcd_init();   // Initialize the the LCD screen.  This also clears the screen.
  cyBot_uart_init();  // Initialize UART
  cyBOT_init_Scan(0b0111);

  char my_data;       // Variable to get bytes from Client
  char command[100];  // Buffer to store command from Client
  int index = 0;      // Index position within the command buffer
  cyBOT_Scan_t myScan;

  lcd_printf("Running");

  while(1)
  {

    index = 0;  // Set index to the beginning of the command buffer
    my_data = cyBot_getByte(); // Get first byte of the command from the Client

    // Get the rest of the command until a newline byte (i.e., '\n') received
    while(my_data != '\n' )
    {
      command[index] = my_data;  // Place byte into the command buffer
      index++;
      my_data = cyBot_getByte(); // Get the next byte of the command
    }

    command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
    command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

    switch (command[0])
    {
        case 'w':
            oi_setWheels(100, 100);
            break;
        case 'a':
            oi_setWheels(100, -100);
            break;
        case 's':
            oi_setWheels(-100, -100);
            break;
        case 'd':
            oi_setWheels(-100, 100);
            break;
        case 'f':
            oi_setWheels(0, 0);
            break;
        case 'm':

    }




    // Send a response to the Client (Starter Client expects the response to end with \n)
    // In this case I am just sending back the first byte of the command received and a '\n'
    cyBot_sendByte(command[0]);

    // Only send a '\n' if the first byte of the command is not a '\n',
    // to avoid sending back-to-back '\n' to the client
    if(command[0] != '\n')
    {
      cyBot_sendByte('\n');
    }

  }

  return 0;
}
