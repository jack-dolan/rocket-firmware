/*
 ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <adc_lac.h>
#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "chprintf.h"
#include "shell.h"

#include "lwipthread.h"
#include "web/web.h"

#include "comm.h"
#include "utils.h"

#include "bldc.h"
#include "drv8305.h"

#include "netstream.h"


/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg){
  (void) arg;
  chRegSetThreadName("blinker");
  palSetPad(GPIOC, GPIOC_LED1);
  palClearPad(GPIOE, GPIOE_LED2);
  while (true) {
    palTogglePad(GPIOC, GPIOC_LED1);
    palTogglePad(GPIOE, GPIOE_LED2);
    chThdSleepMilliseconds(125);
  }
}

/*
 * Application entry point.
 */
int main(void){
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - lwIP subsystem initialization using the default configuration.
   */
  halInit();
  chSysInit();
  lwipInit(NULL);

//  sdStart(&SD1, NULL);

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  chThdCreateStatic(wa_http_server, sizeof(wa_http_server), NORMALPRIO + 1,
      http_server, NULL);


  init_adc_lac();
  init_DRV8305(DRV_X);
  init_DRV8305(DRV_Y);

  init_BLDC();

  init_GSE_comm();

  remote_shell();

  while (true) {
    chThdSleepMilliseconds(100);
  }
}
