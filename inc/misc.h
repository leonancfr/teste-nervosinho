#ifndef MISC_H
#define MISC_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/* Prints to logfile the timestamp in "YYYY-MM-DD HH:MM:SS" format */
void get_timestamp(FILE *logfile);

/* Start charlesgo service */
int start_charlesgo();

/* Stop charlesgo service */
int stop_charlesgo();

/* Start pin flash sequence - BOOT pin HIGH / RESET pin toggled */
int start_flash_sequence();

/* Resets STM - BOOT pin LOW / RESET pin toggled */
int stop_flash_sequence();

/* Flash STM through stm32flash lib */
int flash_STM();

/* Downloads batch file from remote repository */
int get_batch_file();

/* Pings remote DNS through ETH interface */
int run_eth_ping();

/* Run OS commands through system */
int os_run_command(char* command);

#endif