#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

/* Termios struct for Canonical mode serial read */
int open_serial(const char *serialport);

/* Termios struct for Canonical mode serial read */
int open_modem(const char *serialport);

#endif