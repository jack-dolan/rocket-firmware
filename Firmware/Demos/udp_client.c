/*
 *  ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/*
 * This file is a modified version of the lwIP web server demo. The original
 * author is unknown because the file didn't contain any license information.
 */

/**
 * @file web.c
 * @brief HTTP server wrapper thread code.
 * @addtogroup WEB_THREAD
 * @{
 */

#include "ch.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#include "lwip/ip_addr.h"
#include "lwip/sockets.h"

#include "string.h"

#include "web.h"

THD_WORKING_AREA(wa_test_ip, WEB_THREAD_STACK_SIZE);
THD_FUNCTION(test_ip, p){
  (void) p;

  int sockfd, newsockfd;
  char buffer [11];

  struct sockaddr_in servaddr, cli_addr;
//  err_t err;
  socklen_t slen = sizeof(servaddr);

  chThdSetPriority(NORMALPRIO + 10);
  chRegSetThreadName("test_ip");

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("192.168.1.2");//or 10?
//  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(4567);

  //bind(sockfd, (const struct sockaddr * )&servaddr, slen);

  strcpy(buffer, "HELLO WORLD");
  socklen_t servlen = sizeof(servaddr);

  while (true) {
//    recvfrom(sockfd, inbuf, 2048, 0, (struct sockaddr * )&cli_addr, &clilen);
    //clilen = sizeof(cli_addr);

    sendto(sockfd, "HELLO WORLD", sizeof("HELLO WORLD"), 0, (struct sockaddr * )&servaddr, servlen);
    chThdSleepMilliseconds(100);
  }
}

/** @} */
