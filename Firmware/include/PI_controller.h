/*
 * PI_controller.h
 *
 *  Created on: Apr 10, 2016
 *      Author: Dean
 */

#ifndef INCLUDE_PI_CONTROLLER_H_
#define INCLUDE_PI_CONTROLLER_H_

typedef struct {
    float kp;
    float ki;
    float goal;
    float errSum;
} PI_Controller_s;

void init_PI(PI_Controller_s *controller, float kp, float ki, float goal);
float run_PI(PI_Controller_s *controller, float input, float dt);
float run_PI_neg(PI_Controller_s *controller, float input, float dt);

#endif /* INCLUDE_PI_CONTROLLER_H_ */
