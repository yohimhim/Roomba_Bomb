/*
 * movement.c
 *
 *  Created on: Feb 7, 2025
 *      Author: aheiar
 */

#include "movement.h"
#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include "main.h"
#include "uart-interrupt.h"



/**
 * Manual Methods
 */
void manual_move_forward(){
    oi_setWheels(100, 100);
}
void manual_move_backward(){
    oi_setWheels(-100, -100);
}
void manual_turn_left(){
    oi_setWheels(100, -100);
}
void manual_turn_right(){
    oi_setWheels(-100, 100);
}
void manual_stop(){
    oi_setWheels(0, 0);
}

 int move_forward (oi_t *sensor_data, double distance, int state){
     int color = 0;
    int bumper = 0;
    int mode =0;

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(100,100); //move forward at full speed
    while (sum < distance) {

    oi_update(sensor_data); //check sensors
    if(state == 0){
        color = get_cliff(sensor_data); // always check for cliffs
            bumper = get_bumper(sensor_data);
            if (color == 1){
              mode =1;
              return mode;
            }

            if (!(bumper || color) == 0){
                return mode;
            }
    }

    sum += sensor_data -> distance; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return mode;

}



 void move_backward (oi_t *sensor_data, double distance){
     // the following code could be put in function move_forward()
      double sum = distance; // distance member in oi_t struct is type double
      oi_setWheels(-100,-100); //move forward at full speed
      while (sum > 0) {

          oi_update(sensor_data); //check sensors
      sum += sensor_data -> distance; // use -> notation since pointer

      }
      oi_setWheels(0,0); //stop
      return;
 }


 //Turns the bot clockwise by the desired angle
  void turn_clockwise(oi_t *sensor, double degrees){

      lcd_init();
      oi_setWheels(-50, 50);
      oi_update(sensor);
      double angle = 0;
      degrees *= -1;
      while(angle > degrees){

          oi_update(sensor);

          angle += sensor->angle;
          }
      oi_setWheels(0, 0);
  }

  void turn_counterclockwise(oi_t *sensor, double degrees){

      oi_setWheels(50, -50);
      oi_update(sensor);
      double angle = 0;
      while(angle < degrees){
          oi_update(sensor);

          angle += sensor->angle;
          }
      oi_setWheels(0, 0);
  }



 void move_right (oi_t *sensor_data, double degrees){
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-100,100); //move forward at full speed
    while (sum - 10 > (-1 * degrees)) {
    oi_update(sensor_data);
    sum += sensor_data -> angle; // use -> notation since pointer

    }
    oi_setWheels(0,0); //stop
    return;

}


 void move_left (oi_t *sensor_data, double degrees){
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(100,-100); //move forward at full speed
    while (sum + 40 < degrees) {
    oi_update(sensor_data);
    sum += sensor_data -> angle; // use -> notation since pointer

    }
    oi_setWheels(0,0); //stop
    return;

 }


 int get_cliff(oi_t *sensor_data){

     char data[64];


     //ZOOOOMMMM NOISE
      //
     unsigned char phantom_notes[16] = {
         48, 55, 60, 48, 55, 60, 48, 55, 60, 62, 60, 58, 57, 58, 60, 48
         // C3, G3, C4 (repeats), then climbs down and resolves
     };

     unsigned char phantom_durations[16] = {
         16, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 12, 24
     };

             // Load song to slot 0
             oi_loadSong(1, 16, phantom_notes, phantom_durations);

             unsigned char error_notes[4] = {
                 76, 67, 60, 50   //
             };

             unsigned char error_durations[4] = {
                 6, 6, 6, 12      // quick drop then end low
             };

             oi_loadSong(2, 4, error_notes, error_durations);





    oi_update(sensor_data);
    int color;



        // set variables for Raw data
        oi_update(sensor_data);

        int csR  = sensor_data->cliffRightSignal;
        int csFR = sensor_data->cliffFrontRightSignal;
        int csL  = sensor_data->cliffLeftSignal;
        int csFL = sensor_data->cliffFrontLeftSignal;

        oi_update(sensor_data);

        if ( (csR  < 200 ||csL  < 200 ||csFR < 200 ||csFL < 200) ) { // check for black tape

        lcd_printf("Black Found");



        oi_update(sensor_data);
        oi_setWheels(0,0); //stop

        if(csFL < 200 && csFR < 200){
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data, "BLACK FOUND");
            uart_sendStr(data);



        }else if(csL < 200 && csFL < 200){
            turn_counterclockwise(sensor_data,45);
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data,"BLACK FOUND");
            uart_sendStr(data);


        }else if(csR < 200 && csFR < 200){
            turn_clockwise(sensor_data,45);
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data,"BLACK FOUND");
            uart_sendStr(data);

        }
        else if(csFL < 200){
            turn_counterclockwise(sensor_data,20);
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data,"BLACK FOUND");
            uart_sendStr(data);


        }else if(csFR < 200){
            turn_clockwise(sensor_data,20);
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data,"BLACK FOUND");
            uart_sendStr(data);


        }
        else if(csL < 200 ){
            turn_counterclockwise(sensor_data,70);
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data,"BLACK FOUND");
            uart_sendStr(data);



        }else if(csR < 200 ){
            turn_clockwise(sensor_data,70);
            oi_setWheels(250,250);
            timer_waitMillis(1500);
            oi_setWheels(0,0);
            oi_update(sensor_data);
            sprintf(data,"BLACK FOUND");
            uart_sendStr(data);


        }

        oi_play_song(1);
        timer_waitMillis(3000);



        color = 1;
        }
        else if ( csR  >= 2690 ||csL  >= 2690 ||csFR >= 2690 ||csFL >= 2690) {// check for white tape

        lcd_printf("White Found");

        oi_update(sensor_data);
          oi_setWheels(0,0); //stop

          if(csL >= 2690 && csFL >=2690){
              sprintf(data,"Cliff:2,3");
              uart_sendStr(data);

          }else if(csL >= 2690 && csFL >=2690){
              sprintf(data,"Cliff:1,2");
              uart_sendStr(data);

          }else if(csR >= 2690 && csFR >= 2690){
              sprintf(data,"Cliff:3,4");
              uart_sendStr(data);

          }
          else if(csFL >= 2690){
              sprintf(data,"Cliff:2");
              uart_sendStr(data);

          }else if(csFR >= 2690){
              sprintf(data,"Cliff:3");
              uart_sendStr(data);

          }
          else if(csL >= 2690 ){
              sprintf(data,"Cliff:1");
              uart_sendStr(data);

          }else if(csR >= 2690 ){
              sprintf(data,"Cliff:4");
              uart_sendStr(data);

          }

        color = 2;
        oi_update(sensor_data);
        oi_setWheels(-70,-70);
        timer_waitMillis(1000);
        oi_setWheels(0,0);
        oi_play_song(2);





        }
        else{
            color = 0;
        }

    return color;

 }

 int get_bumper(oi_t *sensor_data){
     oi_update(sensor_data);
     int bumper;
     char data[64];



     if (sensor_data->bumpLeft){ //Left =1
         oi_update(sensor_data);

         move_backward (sensor_data, 60);
         bumper = 1;
         sprintf(data,"Bump:Left");
         uart_sendStr(data);
     }
     else if (sensor_data->bumpRight){ //RIGHT = 2
         oi_update(sensor_data);

         move_backward (sensor_data, 60);
         bumper = 2;
         sprintf(data,"Bump:Right");
         uart_sendStr(data);
     }
     else{
         bumper = 0;
     }

     return bumper;


 }




