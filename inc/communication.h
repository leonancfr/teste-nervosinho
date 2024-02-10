#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/select.h>
#include "config_serial.h"

struct Message
{
    int firmwareVersion;
    int messageType;
    int messageCommand;
    int messageID;
    int messageDataLen;
    char messageData[100];
};

/* Sets global file descriptor for results log file and serial communication with stm*/
void communication_init(FILE *fd,  char* STMserialpath, char* modemSerialPath);

/* Parses each token from a formatted charles message */
int parse_message(char *, struct Message *);

/* Reads STM serial response */
int read_incoming_message(struct Message *receivedMessage);

/* Parses Modem serial response*/ 
int read_AT_command(char* buffer);

/* Sets individual tokens for a formatted charles message */
void set_parameters(struct Message *MessagePtr, int version, int type, int command, int id, char *str);

/* Construct charles message from tokens */
void construct_message(struct Message *, char *);

/* Send message through serial and stores response  */
void send_and_receive_message( struct Message *sentMessage, struct Message *receivedMessage, char *messageBuffer, int firmware_version, int messageType, int messageCommand, int messageID, char *data );

/* Receives Modem serial response */
int receive_AT_command(char* buffer);

/* */
void send_AT_command(const char* command);

#endif