/*
 * hs_adc.h
 *
 *  Created on: Oct 26, 2015
 *      Author: Dean
 */

#ifndef ADC_LAC_H_
#define ADC_LAC_H_

#include "ch.h"
#include "hal.h"

#include "PI_controller.h"

extern float linear_pot;
extern float command_pos;

extern float vbat_sense, temp_board, temp_x, temp_y, cur_sense;

extern int speed_pos_pro_flag;
extern PI_Controller_s pos_PI;
extern int pos_col_flag;

void init_adc_lac(void);

#endif /* ADC_LAC_H_ */
