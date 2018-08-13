/*
 * packet.c
 *
 *  Created on: Sep 9, 2014
 *      Author: Dean
 */

#include <adc_lac.h>
#include "ch.h"
#include "hal.h"

#include "string.h"

#include "utils.h"
#include "comm.h"
#include "bldc.h"
#include "adc_lac.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include "string.h"
#include "web/web.h"
#include "bldc.h."

static msg_t SenderMBbuffer[256];
MAILBOX_DECL(GSEMBox, SenderMBbuffer, 256);

uint32_t streaming = 1;

THD_WORKING_AREA(waGSEListenerThread, 1024);
THD_FUNCTION(GSEListenerThread, p){
  (void) p;
  chRegSetThreadName("gse_listener");
  static uint8_t packet_id = 0;
  static uint8_t packet_len = 0;
  static uint8_t data_index = 0;
  static char data_buf[64];
  static packet_rx_state_t packet_rx_state = PACKET_IDLE;
  static msg_t tempc;
  static int i = 0;

  static int sockfd2;
  static struct sockaddr_in servaddr, cli_addr;
  socklen_t slen = sizeof(servaddr);
  sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("192.168.1.2");
  servaddr.sin_port = htons(2006);
  bind(sockfd2, (const struct sockaddr * )&servaddr, slen);
  socklen_t clilen = sizeof(cli_addr);
  static uint8_t inbuf[128];
  static int bytes_rcvd = 0;

  while (1) {

    bytes_rcvd = recvfrom(sockfd2, inbuf, 128, 0, (struct sockaddr * )&cli_addr,
        &clilen);

    if (bytes_rcvd < 1)
      continue;
    for (i = 0; i < bytes_rcvd; i++) {
      tempc = inbuf[i];

      if ((packet_rx_state == PACKET_IDLE) && (tempc == 85)) {
        packet_rx_state = PACKET_WAIT_FOR_ID;
      }
      else if (packet_rx_state == PACKET_WAIT_FOR_ID) {
        packet_id = tempc;
        packet_rx_state = PACKET_WAIT_FOR_LENGTH;
      }
      else if (packet_rx_state == PACKET_WAIT_FOR_LENGTH) {
        packet_len = tempc;
        if (packet_len > 60)
          packet_len = 60;
        packet_rx_state = PACKET_WAIT_FOR_DATA;
      }
      else if ((data_index != packet_len)
          && (packet_rx_state == PACKET_WAIT_FOR_DATA)) {
        data_buf[data_index++] = tempc;
        if (data_index == packet_len) {
          packet_rx_state = PACKET_WAIT_FOR_END;
        }
      }
      else if ((packet_rx_state == PACKET_WAIT_FOR_END) && (tempc == 127)) {
        data_index = 0;
        packet_rx_state = PACKET_IDLE;
        if (packet("sn")) {
          chMBPost(&GSEMBox, SN_PACKET, TIME_INFINITE);
        }
        else if (packet("start_stream")) {
          streaming = 1;
        }
        else if (packet("stop_stream")) {
          streaming = 0;
        }
        //-------------
        else if (packet("fwdir")) {
          set_direction(0);
        }
        else if (packet("rvdir")) {
          set_direction(1);
        }
        else if (packet("rpm1")) {
          set_speed(400);
        }
        else if (packet("rpm2")) {
          set_speed(800);
        }
        else if (packet("rpm3")) {
          set_speed(1500);
        }
        else if (packet("rpm4")) {
          set_speed(2500);
        }
        else if (packet("start_pos")) {
          set_enable_flag(1);
          pos_col_flag = 1;
        }
        else if (packet("stop_pos")) {
          set_enable_flag(0);
          pos_col_flag = 0;
        }
        else if (packet("start_speed")) {
          set_enable_flag(1);
          pos_col_flag = 0;
        }
        else if (packet("stop_speed")) {
          set_enable_flag(0);
        }
        else if (packet("POS")) {
          char tempbuf[5] = {inbuf[6], inbuf[7], inbuf[8], inbuf[9], 0};
          float val = atod(tempbuf);
          pos_PI.goal = (2.9 - val / 454.54);
          command_pos = pos_PI.goal;
          chMBPost(&GSEMBox, TELEM(command_pos), TIME_IMMEDIATE);
        }
        //------------
//        else {
//          chMBPost(&GSEMBox, LOG(log_invalid_command_from_olympus),
//          TIME_INFINITE);
//        }
      }
      else {
        packet_rx_state = PACKET_IDLE;
        data_index = 0;
        /* TODO: report error */
        (void) packet_id;
      }
    }
  }
}

THD_WORKING_AREA(waGSESenderThread, 1024);
THD_FUNCTION(GSESenderThread, p){

  //UDP (lwip) initialization:

  (void) p;
  static int sockfd;
  static struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("192.168.1.10");
  servaddr.sin_port = htons(2005);
  socklen_t servlen = sizeof(servaddr);
  //End of UDP Init

  (void) p;
  static uint8_t txBuf[256];
  static msg_t curr_packet = 0;
  static int write_ptr = 0;
  static void * data_ptr = NULL;
//  uint8_t * buf_ptr = NULL;
//  static int i;
  chRegSetThreadName("gse_sender");
  while (1) {
    write_ptr = 3;
    chMBFetch(&GSEMBox, &curr_packet, TIME_INFINITE);
    chSysLock();
    txBuf[0] = 85;    // packet start
    txBuf[1] = 0;    // length
    txBuf[2] = (curr_packet >> 8) & 0xFF;    // packet type
    // check if a telem packet
    if ((txBuf[2] == TELEM_PACKET_ID) && (streaming == 1)) {
      txBuf[write_ptr++] = curr_packet & 0xFF;
      switch (txBuf[3]) {
        SEND_PACKET(linear_pot)
                  SEND_PACKET(motor_rpm)
                  SEND_PACKET(curr_duty_f)
                  SEND_PACKET(vbat_sense)
                  SEND_PACKET(temp_board)
                  SEND_PACKET(temp_x)
                  SEND_PACKET(temp_y)
                  SEND_PACKET(cur_sense)
                  SEND_PACKET(command_pos)
                    chSysUnlock();
        default:
          continue;
      }
      write_word(txBuf, &write_ptr, data_ptr);
      get_time(txBuf, &write_ptr);
    } else if ((txBuf[2]) == ACK_PACKET_ID) {
      txBuf[write_ptr++] = curr_packet & 0xFF;
    }
    else if ((txBuf[2]) == SN_PACKET_ID) {
      txBuf[write_ptr++] = 's';
      txBuf[write_ptr++] = 'a';
      txBuf[write_ptr++] = 't';
      txBuf[write_ptr++] = 'u';
      txBuf[write_ptr++] = 'r';
      txBuf[write_ptr++] = 'n';
      txBuf[write_ptr++] = 'v';
      streaming = 1;
    }
    else if (txBuf[1] == LOG_PACKET_ID) {
      switch (curr_packet & 0xFF) {
        default:
          chSysUnlock();
          continue;
      }
      write_str(txBuf, &write_ptr, data_ptr);
    }
    else {
      chSysUnlock();
      continue;
    }
    txBuf[write_ptr++] = 127;
    // do not include start, length, and end
    txBuf[1] = write_ptr - 3;
    chSysUnlock();
    //send via UDP:
    sendto(sockfd, txBuf, write_ptr, 0, (struct sockaddr * )&servaddr, servlen);
  }
}

void init_GSE_comm(void){
  chThdCreateStatic(waGSEListenerThread, sizeof(waGSEListenerThread),
  NORMALPRIO + 10,
      GSEListenerThread, NULL);
  chThdSleepMilliseconds(1000);
  chThdCreateStatic(waGSESenderThread, sizeof(waGSESenderThread),
  NORMALPRIO + 10,
      GSESenderThread, NULL);
}

