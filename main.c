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
//    char command[100];  // Buffer to store command from Client
//    int index = 0;      // Index position within the command buffer

    //cyBOT_SERVO_Cal();
    right_calibration_value = 253750;
    left_calibration_value = 1288000;
    double sum = 0;
    double distance = 0;

    float firstScan;
    float secondScan;

    int atBomb = 0;

    oi_setWheels(0,0);



    //star wars
    //
        unsigned char notes[9] = {
               67, 67, 67, 63, 70, 67, 63, 70, 67
           };

           // Durations (1/64 second units); 32 = 0.5 sec, 16 = 0.25 sec
           unsigned char durations[9] = {
               32, 32, 32, 24, 8, 32, 24, 8, 48
           };

           // Load song to slot 0
           oi_loadSong(0, 9, notes, durations);


    while(1)
    {




        oi_update(sensor_data);


        //flag_monitor(sensor_data);
        if (state == AUTO){

            while (state == AUTO && !atBomb){
                //AUTO MODE DRIVING
                lcd_printf("we are in auto");


                //Return if it reaches the object
                if (distance != 0 && sum >= (distance - 5)){
                    lcd_clear();
                     lcd_printf("dist %f : %f", distance, sum);
                     oi_play_song(0);
                     timer_waitMillis(3000);
                    state = MANUAL;

                    break;
                }

                sum = 0;
                distance = 0;
                     flag_monitor(sensor_data);
                     oi_update(sensor_data);



                     float angleToObject1 =  ping_scan(sensor_data); //Angle from first scan
                     firstScan = get_largest_width();
                     flag_monitor(sensor_data);

                     //If the scan is cancelled, we don't want it to calculate angles
                     if(angleToObject1 == -1){
                         cancel_scan_flag = 0;
                         break;
                     }

                     //Turn 180 degrees
                     oi_setWheels(-100, 100);
                     timer_waitMillis(3200);
                     oi_setWheels(0, 0);

                     float angleToObject2 =  ping_scan(sensor_data); //Angle from second scan
                     secondScan = get_largest_width();
                     flag_monitor(sensor_data);

                     //If the scan is cancelled, we don't want it to calculate angles
                     if(angleToObject2 == -1){
                        cancel_scan_flag = 0;
                      break;
                    }

                     lcd_printf("first: %lf\nsecond: %lf",firstScan, secondScan);

                     //compare the scans
                     if(firstScan > secondScan){
                          //Turn 180 degrees
                        oi_setWheels(-100, 100);
                        timer_waitMillis(3200);
                        oi_setWheels(0, 0);

                     }
                        float angleToObject = ping_scan(sensor_data);
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
                while(sum < distance -5){
                    oi_update(sensor_data);
                    flag_monitor(sensor_data);
                    sum += sensor_data->distance;
                }
                oi_setWheels(0,0);
                }

        }

        else if (state == MANUAL){

            while (state == MANUAL){
                oi_update(sensor_data);
                int mode = 0;

                //GET COLOR VALUES
//                int csR  = sensor_data->cliffRightSignal;
//                int csFR = sensor_data->cliffFrontRightSignal;
//                int csL  = sensor_data->cliffLeftSignal;
//                int csFL = sensor_data->cliffFrontLeftSignal;
//                lcd_printf("L:%d FL:%d  FR:%d  R:%d ", csL, csFL, csFR, csR);

                lcd_printf("we are in manual");
                //MANUAL MODE DRIVING
                flag_monitor(sensor_data);
                //index = 0;  // Set index to the beginning of the command buffer
                my_data = cyBot_getByte(); // Get first byte of the command from the Client

                switch (my_data)
                {
                case 'w':

                    mode = move_forward(sensor_data, 40); //10cm
                    if(mode == 1){

                        state = AUTO;
                    }
                    break;
                case 'a':
                    turn_counterclockwise(sensor_data, 45);
                    break;
                case 's':
                    move_backward(sensor_data, 40); //10cm
                    break;
                case 'd':
                    turn_clockwise(sensor_data, 45);
                    break;
                case 'f':
                    oi_setWheels(0, 0);
                    break;
                default:
                    break;
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
