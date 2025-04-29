#include <stdint.h>
#include "uart-interrupt.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "stdbool.h"
#include "main.h"
#include "scan.h"
#include "adc.h"



#warning "Possible unimplemented functions"

enum driver_state_t state = MANUAL;


    void main() {
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    timer_init();
    lcd_init();
    uart_interrupt_init();

    adc_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0111);

    char my_data;       // Variable to get bytes from Client
    char command[100];  // Buffer to store command from Client
    int index = 0;      // Index position within the command buffer

    //cyBOT_SERVO_Cal();
    right_calibration_value = 253750;
    left_calibration_value = 1288000;
    double sum = 0;
    double distance = 0;

    int atBomb = 0;

    oi_setWheels(0,0);


    while(1)
    {


        //flag_monitor(sensor_data);
        if (state == AUTO){

            while (state == AUTO && !atBomb){
                //AUTO MODE DRIVING


                //Return if it reaches the object
                if (distance != 0 && sum >= (distance - 10)){
                    lcd_clear();
                     lcd_printf("dist %f : %f", distance, sum);
                    state = MANUAL;

                    break;
                }

                sum =0;
                distance =0;
                     flag_monitor(sensor_data);
                     oi_update(sensor_data);


                     float angleToObject =  ping_scan(sensor_data); //Angle from scan
                     flag_monitor(sensor_data);

                     //If the scan is cancelled, we don't want it to calculate angles
                     if(angleToObject == -1){
                         cancel_scan_flag = 0;
                         break;
                     }

                     distance = ir_scan() * 3;




                     if (angleToObject < 90){ //turn to the right
                         angleToObject =  90 - angleToObject; //Angle to move the bot right
                         move_right (sensor_data, angleToObject);
                     }
                     else if (angleToObject > 90) {
                         angleToObject = angleToObject - 90;  //Angle to move the bot left
                         move_left (sensor_data, angleToObject);
                     }
                     angleToObject = 0;



                oi_setWheels(100,100);
                while(sum < distance -10){
                    oi_update(sensor_data);
                    flag_monitor(sensor_data);
                    sum += sensor_data->distance;
                }
                oi_setWheels(0,0);
                }

        }

        else if (state == MANUAL){

            while (state == MANUAL){
                //MANUAL MODE DRIVING
                flag_monitor(sensor_data);

                command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
                command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

                switch (command[0])
                {
                    case 'w':
                        manual_move_forward();
                    case 'a':
                        manual_turn_left();
                    case 's':
                        manual_move_backward();
                    case 'd':
                        manual_turn_right();
                    case 'f':
                        manual_stop();
                    case 'm':

                }
            }
        }
    }

    oi_free(sensor_data); // do this once at end of main()

}

void flag_monitor(oi_t *sensor_data){
    //start scan
    //also includes cancel scan flag check within scan.c
    if (ping_scan_flag == 1){
        lcd_clear();
        lcd_puts("Scanning...");
        float x = ping_scan(sensor_data);
        lcd_clear();

        ping_scan_flag = 0;
        cancel_scan_flag = 0;
    }

    //ir scan
    if (ir_scan_flag == 1){
            lcd_clear();
            lcd_puts("IR scan...");
            ir_scan();
            ir_scan_flag = 0;
            cancel_scan_flag = 0;
        }

    //force stop
    if (force_stop_flag == 1){
        lcd_clear();
        lcd_puts("FORCE STOP.");
        //CANCEL ALL ACTIONS
        oi_setWheels(0, 0); //stops wheels
        //flags that cancel scans, sensors, etc are set to 1 in uart-interrupt.c
        //under force_stop_flag which is == 'c' if statement
        //which causes a force stop
        //then they must be reset here:
        ping_scan_flag = 0;
        cancel_scan_flag = 0;
        force_stop_flag = 0;
    }

    //toggle driving modes
    if (auto_flag == 1){
        lcd_clear();
        lcd_puts("SWITCHED TO: '\n' AUTO MODE.");

        state = AUTO;
        auto_flag = 0;
    }
    if (manual_flag == 1){
        lcd_clear();
        lcd_puts("SWITCHED TO: '\n' MANUAL MODE.");

        state = MANUAL;
        manual_flag = 0;
    }

}
