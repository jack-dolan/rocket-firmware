#include "PI_controller.h"

void init_PI(PI_Controller_s *controller, float kp, float ki, float goal) {
  controller->kp = kp;
  controller->ki = ki;
  controller->errSum = 0;
  controller->goal = goal;
}

float run_PI(PI_Controller_s *controller, float input, float dt){
  float error = controller->goal - input;
  controller->errSum += error * dt;
  return (controller->kp * error + controller->errSum * controller->ki);
}

float run_PI_neg(PI_Controller_s *controller, float input, float dt){
  float error = input - controller->goal;
  controller->errSum += error * dt;
  return (controller->kp * error + controller->errSum * controller->ki);
}

