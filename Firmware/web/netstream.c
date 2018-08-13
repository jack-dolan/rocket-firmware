//#include "netstream.h"
//#include "lwip/api.h"
//
//#include "ch.h"
//#include "hal.h"
//
//#include "cmds.h"
//
//#include "netstream.h"
//#include "lwip/sockets.h"
//
//static size_t writes(void *ip, const uint8_t *bp, size_t n) {
//  NetStream *sp = ip;
//
//  return lwip_write(sp->socket, bp, n);
//}
//
//static size_t reads(void *ip, uint8_t *bp, size_t n) {
//  NetStream *sp = ip;
//
//  return lwip_read(sp->socket, bp, n);
//}
//
//static msg_t put(void *ip, uint8_t b) {
//  return (writes(ip, &b, 1) == 1 ? Q_OK : Q_RESET);
//}
//
//static msg_t get(void *ip) {
//  uint8_t b;
//
//  return (reads(ip, &b, 1) == 1 ? b : Q_RESET);
//}
//
//static const struct NetStreamVMT vmt = {writes, reads, put, get};
//
//void nsObjectInit(NetStream *sp, int socket) {
//
//  sp->vmt  = &vmt;
//  sp->socket = socket;
//
//}
//
//static NetStream *nsp;
//
//static const ShellConfig shell_cfg = {
//    (BaseSequentialStream *) &nsp,
//    commands
//};
//
//
//
//static THD_WORKING_AREA(waRemoteShell, 128);
//static THD_FUNCTION(RemoteShell, arg){
//  (void) arg;
//   uint16_t port = REMOTE_SHELL_PORT;
//   struct netconn *conn, *newconn;
//   err_t err;
//   thread_t *shelltp = NULL;
//
//   chRegSetThreadName("remote shell");
//
//   /* Create a new TCP connection handle */
//   conn = netconn_new(NETCONN_TCP);
//   LWIP_ERROR("TCP server: invalid conn", (conn != NULL), return;);
//
//   /* Bind to a port. */
//   netconn_bind(conn, NULL, port);
//
//   /* Listen for connections. */
//   netconn_listen(conn);
//
//   shellInit();
//
//   while (TRUE) {
//      err = netconn_accept(conn, &newconn);
//      if (err != ERR_OK)
//         continue;
//      if(!shelltp) {
//         nsObjectInit(nsp);
//         nsStart(nsp, newconn);
//         shelltp = shellCreate(&shell_cfg, SHELL_WA_SIZE, NORMALPRIO-1);
//      }
////      else if (chThdTerminatedX(shelltp)) {
////        chThdRelease(shelltp); /* Recovers memory of the previous shell.   */
////        shelltp = NULL; /* Triggers spawning of a new shell.        */
////        netconn_delete(newconn);
////      }
//   }
//}
//
//void remote_shell() {
//  chThdCreateStatic(waRemoteShell, sizeof(waRemoteShell), NORMALPRIO-1,
//      RemoteShell, NULL);
//}

#include "netstream.h"
#include "lwip/api.h"

#include "ch.h"
#include "hal.h"

#include "cmds.h"

#define _net_stream_data \
   _base_sequential_stream_data \
   struct netconn * conn; \
   struct netbuf * inbuf; \
   size_t in_offset;

struct NetStreamVMT {
   _base_sequential_stream_methods
};

typedef struct {
   const struct NetStreamVMT *vmt;_net_stream_data
} NetStream;

static size_t write(void *ip, const uint8_t *bp, size_t n) {
   NetStream *sp = ip;

   return netconn_write_partly(sp->conn, bp, n, NETCONN_COPY, NULL);
}

static size_t read(void *ip, uint8_t *bp, size_t n) {
   NetStream *sp = ip;
   err_t err;

   /* If last input buffer was completely consumed, wait for a new packet. */
   while (sp->inbuf == NULL) {
      /* Wait for new packet. */
      err = netconn_recv(sp->conn, &sp->inbuf);
      if (err != ERR_OK) {
         /* Connection closed (or any other errors). */
         return 0;
      }
   }

   netbuf_copy_partial(sp->inbuf, bp, n, sp->in_offset);
   sp->in_offset += n;

   /* Check if there is more data to read. */
   if (sp->in_offset >= netbuf_len(sp->inbuf)) {
      n -= (sp->in_offset - netbuf_len(sp->inbuf));
      netbuf_delete(sp->inbuf);
      sp->in_offset = 0;
      sp->inbuf = NULL;
   }

   return n;
}

static msg_t put(void *ip, uint8_t b) {
   return (write(ip, &b, 1) == 1 ? Q_OK : Q_RESET);
}

static msg_t get(void *ip) {
   uint8_t b;

   return (read(ip, &b, 1) == 1 ? b : Q_RESET);
}

static const struct NetStreamVMT vmt = { write, read, put, get };

static void nsObjectInit(NetStream *sp) {

   sp->vmt = &vmt;
   sp->inbuf = NULL;
   sp->in_offset = 0;
}

static void nsStart(NetStream *sp, struct netconn * conn) {
   sp->conn = conn;
}

/*
* TCP server thread.
*/

NetStream nsp;

static const ShellConfig shell_cfg = {
    (BaseSequentialStream *) &nsp,
    commands
};



static THD_WORKING_AREA(waRemoteShell, 128);
static THD_FUNCTION(RemoteShell, arg){
  (void) arg;
   uint16_t port = REMOTE_SHELL_PORT;
   struct netconn *conn, *newconn;
   err_t err;
   thread_t *shelltp = NULL;

   chRegSetThreadName("remote shell");

   /* Create a new TCP connection handle */
   conn = netconn_new(NETCONN_TCP);
   LWIP_ERROR("TCP server: invalid conn", (conn != NULL), return;);

   /* Bind to a port. */
   netconn_bind(conn, NULL, port);

   /* Listen for connections. */
   netconn_listen(conn);

   shellInit();

   while (TRUE) {
      err = netconn_accept(conn, &newconn);
      if (err != ERR_OK)
         continue;
      if(!shelltp) {
         nsObjectInit(&nsp);
         nsStart(&nsp, newconn);
         shelltp = shellCreate(&shell_cfg, SHELL_WA_SIZE, NORMALPRIO-1);
      } else if (chThdTerminatedX(shelltp)) {
        chThdRelease(shelltp); /* Recovers memory of the previous shell.   */
        nsObjectInit(&nsp);
        nsStart(&nsp, newconn);
        shelltp = shellCreate(&shell_cfg, SHELL_WA_SIZE, NORMALPRIO-1);
      } else {
        netconn_delete(newconn);
      }
   }
}

void remote_shell() {
  chThdCreateStatic(waRemoteShell, sizeof(waRemoteShell), NORMALPRIO-1,
      RemoteShell, NULL);
}
