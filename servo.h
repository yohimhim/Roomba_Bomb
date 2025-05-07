/*
 * servo.h
 *
 *  Created on: Apr 11, 2025
 *      Author: aheiar
 */





#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>


void servo_INIT(void);
void servo_spin(float degrees);
void set_calibrationVals(float left, float right);
void servo_calibrate(void);

#endif /* SERVO_H_ */
