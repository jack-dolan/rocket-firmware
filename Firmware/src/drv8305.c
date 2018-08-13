/*
 * drv8305.c
 *
 *  Created on: Dec 13, 2015
 *      Author: Dean
 */

#include "ch.h"
#include "hal.h"

#include "drv8305.h"

static SPIConfig spicfg_DRV8305 = {NULL, GPIOD,
    GPIOD_DRV_CS_X, SPI_CR1_DFF | SPI_CR1_CPHA | SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0};

void init_DRV8305(int axis){
  spiStart(&SPID3, &spicfg_DRV8305);
  write_DRV8305(DRV8305_HS_DRIVER, 0x3A8, axis);
  write_DRV8305(DRV8305_LS_DRIVER, 0x3A8, axis);
  write_DRV8305(DRV8305_GATE_DRIVER, 0x216, axis);
  write_DRV8305(DRV8305_ICOP, 0x0A0, axis);
  write_DRV8305(DRV8305_SHUNT, 0x02A, axis);
  write_DRV8305(DRV8305_VREG, 0x10A, axis);
  write_DRV8305(DRV8305_VDSSENSE, 0x0C8, axis);
}

uint16_t read_DRV8305(uint16_t addr, int axis) {
  uint16_t read;

  addr = (addr << 11) | 0x8000;

  osalDbgAssert(SPID3.state == SPI_READY, "not ready");

   if (axis == DRV_X) {
     palClearPad(GPIOD, GPIOD_DRV_CS_X);
   } else if (axis == DRV_Y) {
     palClearPad(GPIOE, GPIOE_DRV_CS_Y);
   } else {
     osalDbgAssert((axis == DRV_X) || (axis == DRV_Y), "incorrect drv cs");
   }

   spiExchange(&SPID3, 1, &addr, &read);

   if (axis == DRV_X) {
    palSetPad(GPIOD, GPIOD_DRV_CS_X);
  } else if (axis == DRV_Y) {
    palSetPad(GPIOE, GPIOE_DRV_CS_Y);
  }

  return read;
}

uint16_t write_DRV8305(uint16_t addr,uint16_t val,int axis) {
  uint16_t read;
  uint16_t write;

  write = (addr << 11) | val;

  osalDbgAssert(SPID3.state == SPI_READY, "not ready");

  if (axis == DRV_X) {
    palClearPad(GPIOD, GPIOD_DRV_CS_X);
  } else if (axis == DRV_Y) {
    palClearPad(GPIOE, GPIOE_DRV_CS_Y);
  } else {
    osalDbgAssert((axis == DRV_X) || (axis == DRV_Y), "incorrect drv cs");
  }

  spiExchange(&SPID3, 1, &write, &read);

    if (axis == DRV_X) {
    palSetPad(GPIOD, GPIOD_DRV_CS_X);
  } else if (axis == DRV_Y) {
    palSetPad(GPIOE, GPIOE_DRV_CS_Y);
  }

  return read;
}

