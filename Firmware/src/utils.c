/*
 * utils.c
 *
 *  Created on: Nov 20, 2014
 *      Author: Dean
 */

#include <ctype.h>

#include "utils.h"
#include "comm.h"

float averagef(float *buf,uint8_t num,uint8_t incr){
  float temp = 0;
  uint8_t i;
  for (i = 0; i < num * incr; i += incr)
    temp += buf[i];
  return (float) temp / num;
}

uint32_t average32(uint32_t *buf,uint8_t num,uint8_t incr){
  uint32_t temp = 0;
  uint8_t i;
  for (i = 0; i < num * incr; i += incr)
    temp += buf[i];
  return temp / num;
}

uint32_t average16(uint16_t *buf,uint8_t num,uint8_t incr){
  uint32_t temp = 0;
  uint8_t i;
  for (i = 0; i < (num * incr); i += incr)
    temp += buf[i];
  return temp / num;
}

char UART_CmdEquals(char * UART_Buffer,const char * str,char len){
  int i = 0;
  for (i = 0; i < len; i++) {
    if (str[i] != UART_Buffer[i]) {
      return 0;
    }
  }
  return 1;
}

void get_time(uint8_t * txBuf,int * write_ptr){
  uint32_t cur_time = chVTGetSystemTimeX();
  uint8_t * time_ptr = (uint8_t*) &cur_time;
  txBuf[(*write_ptr)++] = time_ptr[3];
  txBuf[(*write_ptr)++] = time_ptr[2];
  txBuf[(*write_ptr)++] = time_ptr[1];
  txBuf[(*write_ptr)++] = time_ptr[0];
}

void write_word(uint8_t * txBuf,int * write_ptr,void * data){
  uint8_t * data_ptr = (uint8_t*) data;
  txBuf[(*write_ptr)++] = data_ptr[0];
  txBuf[(*write_ptr)++] = data_ptr[1];
  txBuf[(*write_ptr)++] = data_ptr[2];
  txBuf[(*write_ptr)++] = data_ptr[3];
}

void write_str(uint8_t * txBuf,int * write_ptr,const char * str){
  int i = 0;
  while (str[i] != 0x00)
    txBuf[(*write_ptr)++] = str[i++];
}

double atod(char *s)
{
  double a = 0.0;
  int e = 0;
  int c;
  while ((c = *s++) != '\0' && isdigit(c)) {
    a = a * 10.0 + (c - '0');
  }
  if (c == '.') {
    while ((c = *s++) != '\0' && isdigit(c)) {
      a = a * 10.0 + (c - '0');
      e = e - 1;
    }
  }
  if (c == 'e' || c == 'E') {
    int sign = 1;
    int i = 0;
    c = *s++;
    if (c == '+')
      c = *s++;
    else if (c == '-') {
      c = *s++;
      sign = -1;
    }
    while (isdigit(c)) {
      i = i * 10 + (c - '0');
      c = *s++;
    }
    e += i * sign;
  }
  while (e > 0) {
    a *= 10.0;
    e--;
  }
  while (e < 0) {
    a *= 0.1;
    e++;
  }
  return a;
}
