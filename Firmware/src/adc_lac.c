/*
 * high_speed_adc.c
 *
 *  Created on: Oct 26, 2015
 *      Author: Dean
 */

#include "adc_lac.h"
#include "utils.h"
#include "comm.h"
#include "string.h"
#include "bldc.h"
#include "math.h"
#include "PI_controller.h"

int speed_pos_pro_flag = 0;
int pos_col_flag = 0;

float command_pos = 0;

PI_Controller_s pos_PI;

static void bldc_pot(float linear_pot){
  if (speed_pos_pro_flag)
    set_speed(3000 * (1 - fabs(1.8 - linear_pot) / 1.2));
  else if (pos_col_flag) {
    float error = pos_PI.goal - linear_pot;
    if (error > 0.01) {
      float speed = run_PI(&pos_PI, linear_pot, 0.001);
      set_direction(DIR_REV);
      set_speed(speed);
      set_enable_flag(1);
    } else if (error < -0.01) {
      float speed = run_PI_neg(&pos_PI, linear_pot, 0.001);
      set_direction(DIR_FWD);
      set_speed(speed);
      set_enable_flag(1);
    } else {
      pos_PI.errSum = 0;
      set_enable_flag(0);
    }
  }

  if (linear_pot > 3.0)
    set_direction(DIR_FWD);
  else if (linear_pot < 0.6)
    set_direction(DIR_REV);
}

static void adcerrorcallback(ADCDriver *adcp,adcerror_t err){
  (void) adcp;
  (void) err;
}

static const GPTConfig gpt3cfg = {
    4000000, /* 4 MHz timer clock.*/
    NULL, /* Timer callback.*/
    0,
    0
};

#define ADC_BUF_SIZE 32

//#define ADC_DMA_BUF_SIZE ADC_BUF_SIZE*2

uint16_t adc_buf[ADC_BUF_SIZE];

float linear_pot = 0;

//static BSEMAPHORE_DECL(hs_adc_sem, 1);

static void hs_adc_cb(ADCDriver *adcp,adcsample_t *buffer,size_t n){
  (void) adcp;
  uint8_t i = 0;
  uint32_t accum = 0;
  chSysLockFromISR();
  for (i = 0; i < n; i++)
    accum += buffer[i];
  accum /= n;
  linear_pot = (float) accum / 4095.0 * 3.3;
  chMBPostI(&GSEMBox, TELEM(linear_pot));
  bldc_pot(linear_pot);
  chSysUnlockFromISR();
}

static const ADCConversionGroup pot_fb_adcgrpcfg = {
TRUE,
    1,
    hs_adc_cb,
    adcerrorcallback,
    0, /* CR1 */
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0, /* CR2 */
    ADC_SMPR1_SMP_AN12(ADC_SAMPLE_480), /* SMPR1 */
    0, /* SMPR2 */
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN12)
};

//THD_WORKING_AREA(wa_hs_ADCthread, 256);
//THD_FUNCTION(hs_ADCthread, p){
//  (void) p;
//  chRegSetThreadName("hs_adc");
//  adcStartConversion(&ADCD2, &adcgrpcfg2, fake_daq_dma_buf, FAKE_DAQ_DMA_BUF_SIZE);
//  gptStartContinuous(&GPTD3, 100);
//  while (1) {
//    chBSemWait(&hs_adc_sem);
//    chMBPost(&GSEMBox, FAST_DAQ(hermes_temp), TIME_INFINITE);
//  }
//}

/******************************************************************************/
/*********************************  AUX ***************************************/

#define AUX_ADC_BUF_SIZE 16

static adcsample_t aux_adc_buf[AUX_ADC_BUF_SIZE];

float vbat_sense, temp_board, temp_x, temp_y, cur_sense;

static uint8_t adc_step = 0;

static virtual_timer_t aux_adc_vt;

static void aux_adc_cb(ADCDriver *adcp,adcsample_t *buffer,size_t n){
  (void) adcp;
  uint8_t i = 0;
  uint32_t accum = 0;
  chSysLockFromISR();
  for (i = 0; i < n; i++)
    accum += buffer[i];
  accum /= n;

  switch (adc_step) {
    case 0:
      temp_board = (float) accum * 3.3f / 4095.0f / 0.01f - 50.0f;
      chMBPostI(&GSEMBox, TELEM(temp_board));
      palSetPad(GPIOE, GPIOE_AUX_A0);
      palSetPad(GPIOB, GPIOB_AUX_A1);
      palClearPad(GPIOE, GPIOE_AUX_A2);
      adc_step = 1;
      break;

    case 1:
      temp_y = (float) accum * 3.3f / 4095.0f / 0.01f - 50.0f;
      chMBPostI(&GSEMBox, TELEM(temp_y));
      palSetPad(GPIOE, GPIOE_AUX_A0);
      palClearPad(GPIOB, GPIOB_AUX_A1);
      palSetPad(GPIOE, GPIOE_AUX_A2);
      adc_step = 2;
      break;

    case 2:
      cur_sense = (float) accum * 3.3f / 4.095f / 100.0f / 0.1f; // readout in mA
      chMBPostI(&GSEMBox, TELEM(cur_sense));
      palClearPad(GPIOE, GPIOE_AUX_A0);
      palSetPad(GPIOB, GPIOB_AUX_A1);
      palSetPad(GPIOE, GPIOE_AUX_A2);
      adc_step = 3;
      break;

    case 3:
      vbat_sense = (float) accum * 3.3f / 4095.0f * 10.0f;
      chMBPostI(&GSEMBox, TELEM(vbat_sense));
      palSetPad(GPIOE, GPIOE_AUX_A0);
      palSetPad(GPIOB, GPIOB_AUX_A1);
      palSetPad(GPIOE, GPIOE_AUX_A2);
      adc_step = 4;
      break;

    case 4:
      temp_x = (float) accum * 3.3f / 4095.0f / 0.01f - 50.0f;
      chMBPostI(&GSEMBox, TELEM(temp_x));
      palClearPad(GPIOE, GPIOE_AUX_A0);
      palSetPad(GPIOB, GPIOB_AUX_A1);
      palClearPad(GPIOE, GPIOE_AUX_A2);
      adc_step = 0;
      break;

    default:
      adc_step = 0;
  }

  chSysUnlockFromISR();
}

static const ADCConversionGroup aux_adcgrpcfg = {
FALSE,
    1,
    aux_adc_cb,
    0,
    0, /* CR1 */
    ADC_CR2_SWSTART, /* CR2 */
    ADC_SMPR1_SMP_AN10(ADC_SAMPLE_480), /* SMPR1 */
    0, /* SMPR2 */
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
};

static void aux_adc_vt_cb(void *arg){
  (void) arg;
  chSysLockFromISR();
  adcStartConversionI(&ADCD2, &aux_adcgrpcfg, aux_adc_buf, AUX_ADC_BUF_SIZE);
  chVTSetI(&aux_adc_vt, MS2ST(20), aux_adc_vt_cb, NULL);
  chSysUnlockFromISR();
}

/******************************************************************************/
/*********************************  ADC ***************************************/

void init_adc_lac(){
  adcStart(&ADCD1, NULL);

  gptStart(&GPTD3, &gpt3cfg);
  GPTD3.tim->CR2 |= TIM_CR2_MMS_1;

  adcStartConversion(&ADCD1, &pot_fb_adcgrpcfg, adc_buf, ADC_BUF_SIZE);
  gptStartContinuous(&GPTD3, 250);

  init_PI(&pos_PI, 2500, 450, 1.8);

  // first set up mux to read board_temp
  palClearPad(GPIOE, GPIOE_AUX_A0);
  palSetPad(GPIOB, GPIOB_AUX_A1);
  palClearPad(GPIOE, GPIOE_AUX_A2);

  adcStart(&ADCD2, NULL);

  chVTObjectInit(&aux_adc_vt);
  chVTSet(&aux_adc_vt, MS2ST(20), aux_adc_vt_cb, NULL);
}

