/*
 * bldc.h
 *
 *  Created on: Dec 12, 2015
 *      Author: Dean
 */

#ifndef BLDC_H_
#define BLDC_H_

#include "PI_controller.h"

//#define READ_LIMIT_SWITCH_LOWER() palReadPad(GPIOD, 6)
//#define READ_LIMIT_SWITCH_UPPER() palReadPad(GPIOD, 7)

#define READ_LIMIT_SWITCH_LOWER() 0
#define READ_LIMIT_SWITCH_UPPER() 0

#define READ_HALL1() palReadPad(GPIOD, 14)
#define READ_HALL2() palReadPad(GPIOD, 4)
#define READ_HALL3() palReadPad(GPIOD, 5)

#define DIR_FWD 0
#define DIR_REV 1

extern float motor_rpm;
extern float curr_duty_f;

extern int delay_enable;

extern PI_Controller_s motor_PI;

extern int direction;

void init_BLDC(void);

void set_direction(int dir);
void set_duty_cycle(uint16_t duty);
void set_enable_flag(int flag);
float get_motor_speed(void);
void set_speed(float goal_rpm);

#endif /* BLDC_H_ */
