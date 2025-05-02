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

 int move_forward (oi_t *sensor_data, double distance){
     int color = 0;
    int bumper = 0;
    int mode =0;
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(100,100); //move forward at full speed
    while (sum < distance) {

    oi_update(sensor_data); //check sensors
    color = get_cliff(sensor_data); // always check for cliffs
    bumper = get_bumper(sensor_data);
    if (color == 1){
      mode =1;
      return mode;
        break;
    }

    if (!(bumper || color) == 0){
        return mode;
        break;
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
      int color = 0;
       int bumper = 0;
      lcd_init();
      oi_setWheels(-50, 50);
      oi_update(sensor);
      double angle = 0;
      degrees *= -1;
      while(angle > degrees){

          oi_update(sensor);
          get_cliff(sensor); // always check for cliffs
              get_bumper(sensor);

              if (!(bumper || color) == 0){
                     break;
                 }
          angle += sensor->angle;
          }
      oi_setWheels(0, 0);
  }

  void turn_counterclockwise(oi_t *sensor, double degrees){
      int color = 0;
           int bumper = 0;
      oi_setWheels(50, -50);
      oi_update(sensor);
      double angle = 0;
      while(angle < degrees){
          oi_update(sensor);
          get_cliff(sensor); // always check for cliffs
          get_bumper(sensor);
          if (!(bumper || color) == 0){
              break;
          }
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


        color = 1;
        }
        else if ( csR  >= 2500 ||csL  >= 2500 ||csFR >= 2500 ||csFL >= 2500 ) {// check for white tape

        lcd_printf("White Found");
        color = 2;
        oi_update(sensor_data);
        oi_setWheels(-70,-70);
        timer_waitMillis(1000);
        oi_setWheels(0,0);



        }
        else{
            color = 0;
        }

    return color;

 }

 int get_bumper(oi_t *sensor_data){
     oi_update(sensor_data);
     int bumper;


     if (sensor_data->bumpLeft){ //Left =1
         oi_update(sensor_data);

         move_backward (sensor_data, 60);
         bumper = 1;
     }
     else if (sensor_data->bumpRight){ //RIGHT = 2
         oi_update(sensor_data);

         move_backward (sensor_data, 60);
         bumper = 2;
     }
     else{
         bumper = 0;
     }

     return bumper;


 }




