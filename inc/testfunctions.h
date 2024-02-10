#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H

#include "communication.h"
#include "config_serial.h"
#include "misc.h"
#include "config.h"

/* Sets appropriate pathes for test log file, STM serial and Modem serial */ 
void testfunctions_init(FILE *fd, char* modemSerialPath, char* STMSerialPath);

/* EEPROM write functions test, writes batch number*/
int write_eeprom(char *message, struct Message *sentMessage, struct Message *receivedMessage);

/* ETH tests, pings remote web and tries to downloand batch number */
int test_eth();

/* Tests modem response and SIM card presence */
int test_modem();

/* Checks number of connected devices and pings them */
int test_ports();

/* Starts test and request eeprom and display results */
int test_STM(char *message, struct Message *sentMessage, struct Message *receivedMessage);

/* */
int flash_STM_sequence();

/* */
void run_tests(FILE *logfile);

/* flagError avoids multiple error titles such as multiple "ETH ERRORS:" in the log file  */
void log_error(bool *flagError, char* errorTitle, char* errorDescription);

/* */
int ping_device(char* ip);

#endif