
#include "communication.h"

FILE *logfd = NULL;
fd_set read_fds; 
fd_set read_modem_fds;

int serialport;
int serialmodem;

struct timeval timeout;
struct timeval timeout_modem;

void communication_init(FILE *fd,  char* STMserialpath, char* modemSerialPath){
    serialport = open_serial(STMserialpath);
    serialmodem = open_modem(modemSerialPath);
    
    logfd = fd;
}

void set_parameters(struct Message *MessagePtr, int version, int type, int command, int id, char *str)
{
    MessagePtr->firmwareVersion = version;
    MessagePtr->messageType = type;
    MessagePtr->messageCommand = command;
    MessagePtr->messageID = id;
    MessagePtr->messageDataLen = strlen(str)+1;
    strcpy(MessagePtr->messageData, str);
}

void construct_message(struct Message *MessagePtr, char *message)
{
    char formatted_message[256];
    snprintf(formatted_message, sizeof(formatted_message),
             "[version:%d;type:%d;command:%d;message_id:%d;data_len:%d;data:%s]",
             MessagePtr->firmwareVersion, MessagePtr->messageType, MessagePtr->messageCommand, MessagePtr->messageID, MessagePtr->messageDataLen, MessagePtr->messageData);
    strcpy(message, formatted_message);
}

int read_incoming_message(struct Message *receivedMessage)
{
    char buf[256];
    int bytes_read = read(serialport, buf, sizeof(buf));

    if (bytes_read > 0)
    {
        buf[bytes_read] = '\0';
        parse_message(buf, receivedMessage);
        printf("Received message: %s\n", buf);
    }
    else
    {
        return 1;
    }

    return 0;
}

int read_AT_command(char* buffer){
    int n = read(serialmodem, buffer, sizeof(buffer) - 1);

    buffer[n] = '\0';

    if (!strstr(buffer, "OK"))
    {
        return 1;
    }
    return 0;
}



int parse_message(char *_buffer, struct Message *receivedMessage)
{
    int values[5];
    char dataBuffer[20];
    int count = sscanf(_buffer, "[version:%d;type:%d;command:%d;message_id:%d;data_len:%d;data:%20[^]]",
                       &values[0], &values[1], &values[2], &values[3], &values[4], dataBuffer);

    if (count >= 5)
    {
        if (values[0] >= 0 && values[0] <= 255 && values[1] >= 0 && values[1] <= 255 &&
            values[2] >= 0 && values[2] <= 255 && values[3] >= 0 && values[3] <= 65535 &&
            values[4] >= 0 && values[4] <= 255)
        {
            receivedMessage->firmwareVersion = values[0];
            receivedMessage->messageType = values[1];
            receivedMessage->messageCommand = values[2];
            receivedMessage->messageID = values[3];
            receivedMessage->messageDataLen = values[4];
            strncpy(receivedMessage->messageData, dataBuffer, sizeof(receivedMessage->messageData));
            receivedMessage->messageData[sizeof(receivedMessage->messageData) - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

void send_and_receive_message( struct Message *sentMessage, struct Message *receivedMessage, char *messageBuffer,
                                int firmware_version, int messageType, 
                                int messageCommand, int messageID, char *data ){

    set_parameters(sentMessage, firmware_version, messageType, messageCommand, messageID, data);
    construct_message(sentMessage, messageBuffer);

    write(serialport, messageBuffer, strlen(messageBuffer));

    printf("Sent message: %s\n", messageBuffer);

    FD_ZERO(&read_fds);
    FD_SET(serialport, &read_fds);
    
    timeout.tv_sec = 3;  // Set timeout in seconds
    timeout.tv_usec = 0;

    while (1) {
        int ready = select(serialport + 1, &read_fds, NULL, NULL, &timeout);

        if (ready == -1) {
            perror("Error in select");
            exit(EXIT_FAILURE);
        } else if (ready == 0) {
            // Timeout occurred
            fprintf(logfd, "!!! STM32 failed to communicate !!!\n");
            break;
        } else {
            if (FD_ISSET(serialport, &read_fds)) {
                read_incoming_message(receivedMessage);
                break;
            }
            
        }
    }
}

int receive_AT_command(char* buffer){
    FD_ZERO(&read_modem_fds);
    FD_SET(serialmodem, &read_modem_fds);
    
    timeout_modem.tv_sec = 5;  // Set timeout in seconds
    timeout_modem.tv_usec = 0;

    while (1) {
        int ready = select(serialmodem + 1, &read_modem_fds, NULL, NULL, &timeout_modem);

        if (ready == -1) {
            perror("Error in select");
            exit(EXIT_FAILURE);
        } else if (ready == 0) {
            // Timeout occurred
            return 1;
        } else {
            if (FD_ISSET(serialmodem, &read_modem_fds)) {
                if(read_AT_command(buffer)){
                    continue;
                }
                break;
            }
            
        }
    }
    return 0;
}

void send_AT_command(const char* command){
    write(serialmodem, command, strlen(command));
}