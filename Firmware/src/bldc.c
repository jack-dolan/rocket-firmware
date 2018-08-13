/*
 * bldc.c
 *
 *  Created on: Dec 12, 2015
 *      Author: Dean
 */

#include "ch.h"
#include "hal.h"

#include "bldc.h"
#include "drv8305.h"

#include "comm.h"
#include "PI_Controller.h"

static uint16_t curr_duty_cycle = 0;
int direction = 0;
static int enable_flag = 0;
static int delay_flag = 0;
float motor_rpm = 0;
float curr_duty_f = 0;
int delay_enable = 0;

PI_Controller_s motor_PI;

const pwmcnt_t DUTY_MIN = 200;
const pwmcnt_t DUTY_MAX = 6000;

static void commutate(int);

static int read_hall(void){
  return READ_HALL1() << 2 | (READ_HALL2() << 1) | (READ_HALL3() << 0);
}

float get_motor_speed(){
  return motor_rpm;
}

void set_direction(int dir){
  if (direction != dir)
    delay_flag = 1;
  direction = dir;
}

void set_duty_cycle(uint16_t duty){
  if (duty > DUTY_MAX)
    duty = DUTY_MAX;
  else if ((duty < DUTY_MIN) && (duty != 0))
    duty = DUTY_MIN;
  curr_duty_cycle = duty;
}

void set_speed(float goal_rpm){
  if (goal_rpm > 10000)
    goal_rpm = 10000;
  motor_PI.goal = goal_rpm;
}

void set_enable_flag(int flag){
  syssts_t sts = chSysGetStatusAndLockX();
  enable_flag = flag;

  if (flag == 1) {
    enable_gate_DRV8305()
    extChannelEnableI(&EXTD1, 4);
    extChannelEnableI(&EXTD1, 5);
    extChannelEnableI(&EXTD1, 14);
    if (READ_LIMIT_SWITCH_LOWER()) {
      direction = 0;
    } else if (READ_LIMIT_SWITCH_UPPER()) {
      direction = 1;
    }
  }

  else {
    extChannelDisableI(&EXTD1, 4);
    extChannelDisableI(&EXTD1, 5);
    extChannelDisableI(&EXTD1, 14);

    disable_gate_DRV8305()
  }
  chSysRestoreStatusX(sts);
}

//static void pwmpcb(PWMDriver *pwmp){
//  (void) pwmp;
//}

static PWMConfig pwmcfg = {
    84000000, /* 84 MHz PWM clock frequency.   */
    4200, /* Initial PWM period 50us.       */
    NULL, //callback
    { //channels
    {PWM_OUTPUT_ACTIVE_HIGH | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_LOW | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW, NULL},
        {PWM_OUTPUT_DISABLED, NULL}
    },
    0,
    //    0x00BD,
    0x003D,
    0
};

//static void limit_switch_cb(EXTDriver *extp,expchannel_t channel){
//  (void) extp;
//  (void) channel;
//
//  return;
//
//  if (READ_LIMIT_SWITCH_LOWER() && direction) {
//    direction = 0;
//    delay_flag = 1;
//  } else if (READ_LIMIT_SWITCH_UPPER() && !direction) {
//    direction = 1;
//    delay_flag = 1;
//  }
//}

static void hall_interrupt(EXTDriver *extp,expchannel_t channel){
  (void) extp;
  (void) channel;

  chSysLockFromISR();
  int hall_curr = read_hall();
  commutate(hall_curr);
  chSysUnlockFromISR();
}

static const EXTConfig extcfg = {
    {
        {EXT_CH_MODE_DISABLED, NULL}, // 0
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_DISABLED | EXT_MODE_GPIOD,
            hall_interrupt}, // 4
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_DISABLED | EXT_MODE_GPIOD,
            hall_interrupt},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}, // 8
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}, // 12
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_DISABLED | EXT_MODE_GPIOD,
            hall_interrupt},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}, // 16
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}, //20
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}
    }
};

static void enablePWMChannel(PWMDriver *pwmp,pwmchannel_t channel,
    pwmcnt_t width){
  switch (channel) {
    case 0:
      pwmp->tim->CCER |= STM32_TIM_CCER_CC1NE;
      break;
    case 1:
      pwmp->tim->CCER |= STM32_TIM_CCER_CC2NE;
      break;
    case 2:
      pwmp->tim->CCER |= STM32_TIM_CCER_CC3NE;
      pwmp->tim->CCER |= STM32_TIM_CCER_CC3E;
      break;
  }
  pwmEnableChannelI(pwmp, channel, width);
}

static void disablePWMChannel(PWMDriver *pwmp,pwmchannel_t channel){
  switch (channel) {
    case 0:
      pwmp->tim->CCER &= ~STM32_TIM_CCER_CC1NE;
      break;
    case 1:
      pwmp->tim->CCER &= ~STM32_TIM_CCER_CC2NE;
      break;
    case 2:
      pwmp->tim->CCER &= ~STM32_TIM_CCER_CC3NE;
      pwmp->tim->CCER &= ~STM32_TIM_CCER_CC3E;
      break;
  }
  pwmDisableChannelI(pwmp, channel);
}

static void commutate(int step){
  if (!enable_flag)
    return;
  switch (step) {
    case 1:
      if (direction) { // 0 - +
        disablePWMChannel(&PWMD8, 0);
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
      } else { // 0 + -
        disablePWMChannel(&PWMD8, 0);
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
      }
      break;

    case 2:
      if (direction) { // - + 0
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        disablePWMChannel(&PWMD8, 1);
      } else { // + - 0
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        disablePWMChannel(&PWMD8, 1);
      }
      break;

    case 3:
      if (direction) { // - 0 +
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        disablePWMChannel(&PWMD8, 2);
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
      } else { // + 0 -
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        disablePWMChannel(&PWMD8, 2);
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
      }
      break;

    case 4:
      if (direction) { // + 0 -
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        disablePWMChannel(&PWMD8, 2);
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
      } else { // - 0 +
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        disablePWMChannel(&PWMD8, 2);
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
      }
      break;

    case 5:
      if (direction) { // + - 0
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        disablePWMChannel(&PWMD8, 1);

      } else { // - + 0
        enablePWMChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        disablePWMChannel(&PWMD8, 1);
      }
      break;

    case 6:
      if (direction) { // 0 + -
        disablePWMChannel(&PWMD8, 0);
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
      } else { // 0 - +
        disablePWMChannel(&PWMD8, 0);
        enablePWMChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 0));
        enablePWMChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, curr_duty_cycle));
      }
      break;

    default:
      disablePWMChannel(&PWMD8, 0);
      disablePWMChannel(&PWMD8, 1);
      disablePWMChannel(&PWMD8, 2);
      enable_flag = 0;
      break;
  }
}

int reset_I = 0;

THD_WORKING_AREA(waBLDCThread, 512);
THD_FUNCTION(BLDCThread, p){
  (void) p;
  chRegSetThreadName("bldc");
  static int hall_cur = 0;
  static int counter = 0;
  while (1) {
    chThdSleepMilliseconds(10);
    if (delay_flag & delay_enable) {
      if (counter++ == 10) {
        counter = 0;
        delay_flag = 0;
        set_duty_cycle(0);
        if (enable_flag) {
          set_enable_flag(1);
          motor_PI.errSum = 0;
        }
      } else {
        extChannelDisable(&EXTD1, 4);
        extChannelDisable(&EXTD1, 5);
        extChannelDisable(&EXTD1, 14);
        disable_gate_DRV8305()
        continue;
      }
    }
    if (enable_flag) {
      enable_gate_DRV8305()
      chSysLock();
      if (curr_duty_cycle == 0) {
        set_duty_cycle((motor_PI.goal + 500.0) / 1.45);
        hall_cur = read_hall();
        commutate(hall_cur);
        reset_I = 1;
      }
      chSysUnlock();
    } else {
      motor_PI.errSum = 0;
      disable_gate_DRV8305()
      disablePWMChannel(&PWMD8, 0);
      disablePWMChannel(&PWMD8, 1);
      disablePWMChannel(&PWMD8, 2);
      set_duty_cycle(0);
    }
  }
}

void eicu_cb(EICUDriver *eicup,eicuchannel_t channel,uint32_t w,uint32_t p){
  (void) eicup;
  (void) w;
  (void) channel;
  chSysLockFromISR();
  float period = (float) p / 100000.0;
  motor_rpm = 60.0 / (period * 2.0);
  chMBPostI(&GSEMBox, TELEM(motor_rpm));
  if (reset_I) {
    motor_PI.errSum = 0;
    reset_I = 0;
  } else {
//    curr_duty_cycle += run_PI(&motor_PI, motor_rpm, period);
    set_duty_cycle(curr_duty_cycle + run_PI(&motor_PI, motor_rpm, period));
  }
  curr_duty_f = (float) curr_duty_cycle;
  chMBPostI(&GSEMBox, TELEM(curr_duty_f));
  chSysUnlockFromISR();
}

static const EICUChannelConfig x_hallcfg = {
    EICU_INPUT_ACTIVE_HIGH,
    EICU_INPUT_EDGE,
    eicu_cb
};

static const EICUConfig eicucfg = {
    100000, /* EICU clock frequency in Hz.*/
    {
        0,
        0,
        &x_hallcfg,
        0
    },
    0
};

void init_BLDC(){
  extStart(&EXTD1, &extcfg);

  pwmStart(&PWMD8, &pwmcfg);

  eicuStart(&EICUD4, &eicucfg);
  eicuEnable(&EICUD4);

  init_PI(&motor_PI, 0.4, 0.009, 250);

  chThdCreateStatic(waBLDCThread, sizeof(waBLDCThread), NORMALPRIO + 1,
      BLDCThread, NULL);
}
