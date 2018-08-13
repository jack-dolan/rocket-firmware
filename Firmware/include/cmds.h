/*
 * cmds.h
 *
 *  Created on: Aug 11, 2014
 *      Author: Dean
 */

#ifndef CMDS_H_
#define CMDS_H_

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#define SHELL_WA_SIZE   2048
#define TEST_WA_SIZE    256

#define SHELL_SD SD3

extern const ShellCommand commands[];

#endif /* CMDS_H_ */
