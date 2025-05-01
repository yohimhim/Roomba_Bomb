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

 void square(oi_t *sensor_data){





 }







