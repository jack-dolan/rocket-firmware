/*
 * utils.h
 *
 *  Created on: Nov 20, 2014
 *      Author: Dean
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "ch.h"
#include "hal.h"

#define DEG_TO_RAD(deg) deg*M_PI/180.0f
#define RAD_TO_DEG(rad) rad*180.0f/M_PI

float averagef(float *buf,uint8_t num,uint8_t incr);
uint32_t average32(uint32_t *buf,uint8_t num,uint8_t incr);
uint32_t average16(uint16_t *buf,uint8_t num,uint8_t incr);

char UART_CmdEquals(char * UART_Buffer,const char * str,char len);
void get_time(uint8_t * txBuf,int *write_ptr);
void write_word(uint8_t * txBuf,int *write_ptr,void *data);
void write_str(uint8_t * txBuf,int * write_ptr,const char * str);
double atod(char *s);
void SAFE(void);

#endif /* UTILS_H_ */
