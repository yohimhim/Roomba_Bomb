/*
 * movement.h
 *
 *  Created on: Feb 7, 2025
 *      Author: aheiar
 */

#include "open_interface.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

/**
 * Manual Movement
 */
void manual_move_forward(void);
void manual_move_backward(void);
void manual_turn_left(void);
void manual_turn_right(void);
void manual_stop(void);

/**
 * Autonomous Movement
 */
int move_forward (oi_t *sensor_data, double distance_mm, int state);
void turn_clockwise(oi_t *sensor, double degrees);
void turn_counterclockwise(oi_t *sensor, double degrees);
void move_right(oi_t *sensor_data, double angle);
void move_left(oi_t *sensor_data, double angle);
void move (oi_t *sensor_data, double distance_mm, double wheel1, double wheel2);
void move_backward (oi_t *sensor_data, double distance_mm);
int get_cliff(oi_t *sensor_data);
int get_bumper(oi_t *sensor_data);


#endif /* MOVEMENT_H_ */
