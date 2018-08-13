#include <adc_lac.h>
#include "cmds.h"

#include "comm.h"
#include "string.h"

#include "drv8305.h"
#include "bldc.h"
#include "adc_lac.h"

static int cmd_equals_check(char* cmd,const char * str,int len){
  int i = 0;
  for (; i < len; i++) {
    if (cmd[i] != str[i]) {
      return 0;
    }
  }
  return 1;
}

#define cmd_equals(cmd, str) cmd_equals_check(cmd, str, strlen(str))

static void scmd_mem(BaseSequentialStream *chp,int argc,char *argv[]){
  size_t n, size;

  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void scmd_threads(BaseSequentialStream *chp,int argc,char *argv[]){
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%08lx %08lx %4lu %4lu %9s %lu\r\n",
        (uint32_t) tp, (uint32_t) tp->p_ctx.r13,
        (uint32_t) tp->p_prio, (uint32_t)(tp->p_refs - 1),
        states[tp->p_state]);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void scmd_test(BaseSequentialStream *chp,int argc,char *argv[]){
  thread_t *tp;

  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(),
      TestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory\r\n");
    return;
  }
  chThdWait(tp);
}

static void scmd_drvx(BaseSequentialStream *chp,int argc,char *argv[]){
  uint16_t addr;
  uint16_t val;
  uint16_t read;

  if (argc < 1)
    return;

  if (cmd_equals(argv[0], "dump")) {
    for (addr = 0; addr <= 0xC; addr++) {
      if (addr == 0x08)
        continue;
      read = read_DRV8305(addr, DRV_X);
      chprintf(chp, "addr 0x%01X:0x%04X\r\n", addr, read);
    }
  } else if (cmd_equals(argv[0], "clear")) {
    write_DRV8305(DRV8305_ICOP, 0x002, DRV_X);
    chprintf(chp, "faults cleared\r\n");
  } else if (argc == 1) {
    addr = strtoul(argv[0], NULL, 0);
    read = read_DRV8305(addr, DRV_X);
    chprintf(chp, "addr 0x%01X: 0x%04X\r\n", addr, read);
  } else if (argc == 2) {
    addr = strtoul(argv[0], NULL, 0);
    val = strtoul(argv[1], NULL, 0);
    read = write_DRV8305(addr, val, DRV_X);
    chprintf(chp, "addr 0x%01X written to: 0x%04X\r\n", addr, val);
  }
}

static void scmd_drvy(BaseSequentialStream *chp,int argc,char *argv[]){
  uint16_t addr;
  uint16_t val;
  uint16_t read;

  if (argc < 1)
    return;

  if (cmd_equals(argv[0], "dump")) {
    for (addr = 0; addr <= 0xC; addr++) {
      if (addr == 0x08)
        continue;
      read = read_DRV8305(addr, DRV_Y);
      chprintf(chp, "addr 0x%01X:0x%04X\r\n", addr, read);
    }
  } else if (cmd_equals(argv[0], "clear")) {
    write_DRV8305(DRV8305_ICOP, 0x002, DRV_Y);
    chprintf(chp, "faults cleared\r\n");
  } else if (argc == 1) {
    addr = strtoul(argv[0], NULL, 0);
    read = read_DRV8305(addr, DRV_Y);
    chprintf(chp, "addr 0x%01X: 0x%04X\r\n", addr, read);
  } else if (argc == 2) {
    addr = strtoul(argv[0], NULL, 0);
    val = strtoul(argv[1], NULL, 0);
    read = write_DRV8305(addr, val, DRV_Y);
    chprintf(chp, "addr 0x%01X written to: 0x%04X\r\n", addr, val);
  }
}

static void scmd_duty(BaseSequentialStream *chp,int argc,char *argv[]){
  uint16_t val;
  if (argc < 1)
    return;
  val = strtoul(argv[0], NULL, 0);
  set_duty_cycle(val);
  chprintf(chp, "Duty cycle set to: %u\r\n", val);
}

static void scmd_dir(BaseSequentialStream *chp,int argc,char *argv[]){
  uint16_t val;
  if (argc < 1)
    return;
  val = strtoul(argv[0], NULL, 0);
  set_direction(val);
  chprintf(chp, "Direction set to: %u\r\n", val);
}

static void scmd_en(BaseSequentialStream *chp,int argc,char *argv[]){
  uint16_t val;
  if (argc < 1)
    return;
  val = strtoul(argv[0], NULL, 0);
  set_enable_flag(val);
  chprintf(chp, "Enable set to: %u\r\n", val);
  if (val != 1) {
    disable_gate_DRV8305()
    ;
    set_enable_flag(0);
  }
}

static void scmd_sw(BaseSequentialStream *chp,int argc,char *argv[]){
  (void) argc;
  (void) argv;

  chprintf(chp, "Limit switch low state:  %d\r\n", READ_LIMIT_SWITCH_LOWER());
  chprintf(chp, "Limit switch high state: %d\r\n\n", READ_LIMIT_SWITCH_UPPER());

  chprintf(chp, "Hall 1 state: %d\r\n", READ_HALL1());
  chprintf(chp, "Hall 2 state: %d\r\n", READ_HALL2());  chprintf(chp, "Hall 3 state: %d\r\n\n", READ_HALL3());
}

static void scmd_speed(BaseSequentialStream *chp,int argc,char *argv[]){
  (void) argc;
  (void) argv;

  chprintf(chp, "Current motor speed: %.2f\r\n", get_motor_speed());
}

static void scmd_set_speed(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1)
    return;

  float val = atod(argv[0]);

  set_speed(val);
  chprintf(chp, "Motor speed set to: %.2f\r\n", val);
}

static void scmd_skp(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    chprintf(chp, "speed PI kp: %f\r\n", motor_PI.kp);
    return;
  }

  float val = atod(argv[0]);

  motor_PI.kp = val;;
  chprintf(chp, "speed PI kp set to: %f\r\n", val);
}

static void scmd_ski(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    chprintf(chp, "speed PI ki: %f\r\n", motor_PI.ki);
    return;
  }

  float val = atod(argv[0]);

  motor_PI.ki = val;;
  chprintf(chp, "speed PI ki set to: %f\r\n", val);
}

static void scmd_pos_speed(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    return;
  }
  int val = strtoul(argv[0], NULL, 0);
  speed_pos_pro_flag = val;
  chprintf(chp, "pos/speed easter egg set to: %f\r\n", val);
}

static void scmd_delay(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    return;
  }
  int val = strtoul(argv[0], NULL, 0);
  delay_enable = val;
  chprintf(chp, "delay enable set to: %d\r\n", val);
}

static void scmd_pos_ctl(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    return;
  }
  int val = strtoul(argv[0], NULL, 0);
  pos_col_flag = val;
  chprintf(chp, "position control set to: %d\r\n", val);
}

static void scmd_pos_goal(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    chprintf(chp, "pos goal: %f\n", pos_PI.goal);
    return;
  }
  float val = atod(argv[0]);
  if (val > 2.95)
    val = 3.0;
  else if (val < 0.6)
    val = 0.65;
  pos_PI.goal = val;
  command_pos = pos_PI.goal;
  chMBPost(&GSEMBox, TELEM(command_pos), TIME_IMMEDIATE);
}

static void scmd_pkp(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    chprintf(chp, "pos PI kp: %f\r\n", pos_PI.kp);
    return;
  }

  float val = atod(argv[0]);
  pos_PI.kp = val;;
  chprintf(chp, "pos PI kp set to: %f\r\n", val);
}

static void scmd_pki(BaseSequentialStream *chp,int argc,char *argv[]){
  if (argc < 1) {
    chprintf(chp, "pos PI ki: %f\r\n", pos_PI.ki);
    return;
  }

  float val = atod(argv[0]);
  pos_PI.ki = val;;
  chprintf(chp, "pos PI ki set to: %f\r\n", val);
}

#define scmd(NAME)  { #NAME, scmd_ ## NAME }

const ShellCommand commands[] = {
scmd(mem),
scmd(threads),
scmd(test),
scmd(drvx),
scmd(drvy),
scmd(dir),
scmd(duty),
scmd(set_speed),
scmd(en),
scmd(sw),
scmd(speed),
scmd(skp),
scmd(ski),
scmd(pos_speed),
scmd(delay),
scmd(delay),
scmd(delay),
scmd(pos_ctl),
scmd(pos_goal),
scmd(pkp),
scmd(pki),
    {NULL, NULL}};
