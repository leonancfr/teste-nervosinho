#include "config_serial.h"

int open_serial(const char *device)
{
    int fd = open(device, O_RDWR | O_NOCTTY);

    if (fd == 1)
    {
        perror("open_serial: Unable to open device\n");
        return 1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetospeed(&options, B115200);
    cfsetispeed(&options, B115200);

    // 8N1 (8 data bits, no parity, 1 stop bit)
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Disable hardware flow control
    options.c_cflag &= ~CRTSCTS;

    // Enable receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Canonical input
    options.c_lflag |= ICANON;

    // Raw output
    options.c_oflag &= ~OPOST;

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &options) < 0)
    {
        perror("open_serial: Couldn't set term attributes\n");
        return 1;
    }

    return fd;
}

int open_modem(const char *usbPath)
{
    int fd = open(usbPath, O_RDWR | O_NOCTTY);

    if (fd == 1)
    {
        perror("open_serial: Unable to open device\n");
        return 1;   
    }

    struct termios options;
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetospeed(&options, B115200);
    cfsetispeed(&options, B115200);

    // 8N1 (8 data bits, no parity, 1 stop bit)
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Disable hardware flow control
    options.c_cflag &= ~CRTSCTS;

    // Enable receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Canonical input
    options.c_lflag |= ICANON;

    options.c_lflag &= ~ECHO;

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &options) < 0)
    {
        perror("open_serial: Couldn't set term attributes\n");
        return 1;
    }

    return fd;
}
