/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* shell.h
*
******************************************************************************/

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "helper.h"

#define READ "r"

void shell(char*);
short execute_commands(char*);
int forkFunction();

#endif