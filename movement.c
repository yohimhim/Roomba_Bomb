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

 void move_forward (oi_t *sensor_data, double distance){
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(100,100); //move forward at full speed
    while (sum < distance) {

    oi_update(sensor_data); //check sensors
    sum += sensor_data -> distance; // use -> notation since pointer
    if (sensor_data->bumpRight || sensor_data->bumpLeft){
        break;
    }

    }
    oi_setWheels(0,0); //stop



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


    if(sensor_data ->cliffLeft || sensor_data ->cliffRight || sensor_data ->cliffFrontLeft || sensor_data ->cliffFrontRight){ //if the sensor is triggered
        // set variables for Raw data
        oi_setWheels(0,0);
        oi_update(sensor_data);

        int csR  = sensor_data->cliffRightSignal;
        int csFR = sensor_data->cliffFrontRightSignal;
        int csL  = sensor_data->cliffLeftSignal;
        int csFL = sensor_data->cliffFrontLeftSignal;

        if ( (csR  < 200 ||csL  < 200 ||csFR < 200 ||csFL < 200) ) { // check for black tape

        lcd_printf("Black Found");


        color = 1;
        }   else if ( csR  >= 2700 ||csL  >= 2700 ||csFR >= 2700 ||csFL >= 2700 ) { // check for white tape

        lcd_printf("White Found");
        color = 2;
        }
        else{
            color = 0;
        }
    }
    return color;

 }

 int get_bumper(oi_t *sensor_data){
     oi_update(sensor_data);
     int bumpedLeft;
     int bumpedRight;

     if (sensor_data->bumpLeft){
         oi_setWheels(0,0); //stop
         bumpedLeft = 1;
     }
     else if (sensor_data->bumpRight){
         oi_setWheels(0,0); //stop
        bumpedRight = 1;
     }
     else {
         bumpedLeft = 0;
         bumpedRight = 0;
     }

 }





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
#include <math.h>


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

 void move_forward (oi_t *sensor_data, double distance){
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(100,100); //move forward at full speed
    while (sum < distance) {

    oi_update(sensor_data); //check sensors
    sum += sensor_data -> distance; // use -> notation since pointer
    if (sensor_data->bumpRight || sensor_data->bumpLeft){
        break;
    }

    }
    oi_setWheels(0,0); //stop



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

 void square(oi_t *sensor_data){





 }

 int get_cliff(oi_t *sensor){
    oi_update(sensor);


    if(sensor ->cliffLeft){

        return sensor ->cliffLeftSignal;
    }
    else if(sensor ->cliffRight){

        return sensor ->cliffRightSignal;
    }
    else if(sensor ->cliffFrontLeft){

        return sensor ->cliffFrontLeftSignal;
    }
    else if(sensor ->cliffFrontRight){

        return sensor ->cliffFrontRightSignal;
    }
    else{

     return 0;

    }
 }




