/*
 * drv8305.h
 *
 *  Created on: Dec 13, 2015
 *      Author: Dean
 */

#ifndef DRV8305_H_
#define DRV8305_H_

#define DRV8305_WARNING_REG 0x01
#define DRV8305_VDSFAULTS   0x02
#define DRV8305_ICFAULTS    0x03
#define DRV8305_VGSFAULTS   0x04
/*      ^^ read only ^^       */
#define DRV8305_HS_DRIVER   0x05
#define DRV8305_LS_DRIVER   0x06
#define DRV8305_GATE_DRIVER 0x07
#define DRV8305_ICOP        0x09
#define DRV8305_SHUNT       0x0A
#define DRV8305_VREG        0x0B
#define DRV8305_VDSSENSE    0x0C


void init_DRV8305(int axis);
uint16_t read_DRV8305(uint16_t addr, int axis);
uint16_t write_DRV8305(uint16_t addr, uint16_t val, int axis);

#define enable_gate_DRV8305() palSetPad(GPIOD, 2);
#define disable_gate_DRV8305() palClearPad(GPIOD, 2);

#define DRV_X 1
#define DRV_Y 2


#endif /* DRV8305_H_ */
