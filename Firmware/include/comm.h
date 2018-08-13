/*
 * packet.h
 *
 *  Created on: Jan 16, 2015
 *      Author: Dean
 */

#ifndef COMM_H_
#define COMM_H_

#include "packet_codes.h"
#include "utils.h"

#define TELEM_PACKET_ID 13
#define TELEM(x) ((TELEM_PACKET_ID << 8) | id_ ## x)

#define FAST_DAQ_PACKET_ID 12
#define FAST_DAQ(x) ((FAST_DAQ_PACKET_ID << 8) | id_ ## x)

#define SN_PACKET_ID  14
#define SN_PACKET 0xE00

#define LOG_PACKET_ID 15
#define LOG(x) ((LOG_PACKET_ID << 8) | id_ ## x)

#define ACK_PACKET_ID 90
#define ACK(x) (0x5A00 | x)

#define CMD_PACKET_ID 12
#define CMD(x) ((CMD_PACKET_ID << 8) | id_ ## x)

#define HEALTH_PACKET 0xB00
#define HEALTH_PACKET_ID 0xB

#define GSE_UARTD UARTD2

 #define SEND_PACKET(data) case id_ ## data : \
          data_ptr = &data; \
          break;

 #define SEND_PACKET_BUF(buf, index) case id_ ## buf ## _ ## index : \
          data_ptr = &buf[index]; \
          break;

#define SEND_CMD(cmd) case id_ ## cmd : \
          data_ptr = cmd; \
          break;

#define SEND_LOG(log) case id_ ## log : \
          data_ptr = log; \
          break;

#define packet(x) UART_CmdEquals(data_buf, x, sizeof(x)-1)
#define cmd_packet(x) UART_CmdEquals(data_buf+1, x, sizeof(x)-1)

typedef enum {
  PACKET_IDLE,
  PACKET_WAIT_FOR_ID,
  PACKET_WAIT_FOR_LENGTH,
  PACKET_WAIT_FOR_DATA,
  PACKET_WAIT_FOR_END,
} packet_rx_state_t;

void init_GSE_comm(void);
extern mailbox_t GSEMBox;

extern float kronos_temp;
extern float jackJack;

extern uint32_t delay_reset;

#endif /* COMM_H_ */
