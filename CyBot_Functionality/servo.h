

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>

#ifndef SERVO_H_
#define SERVO_H_

void servoo_init(void);
void servoo_move(uint16_t degrees);

#endif /* LAB10_CODE_FILES_SERVO_H_ */
