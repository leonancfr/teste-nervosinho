#include "testfunctions.h"

extern FILE *logfd;
extern int serialport;
extern int serialmodem;

int ping_device(char* ip){
    char command[128];
    snprintf(command, sizeof(command), "ping -c 4 %s", ip);
    printf("Executing: %s\n", command);

    if (os_run_command(command) > 0)
    {
        return 1;
    };
    return 0;
}

void log_error(bool *flagError, char* errorTitle, char* errorDescription){
    if (!*flagError)
    {
        fprintf(logfd, errorTitle);
        *flagError = true;
    }
    fprintf(logfd, errorDescription);
}


int test_eth()
{
    bool flagError = false;

    if (run_eth_ping())
    {
        log_error(&flagError, "ETH ERRORS: \n", "\t- Couldn't ping DNS server (either blocked or no available network)\n");
        flagError = true;
    }

    if (get_batch_file())
    {
        log_error(&flagError, "ETH ERRORS: \n", "\t- Couldn't download batch information text file\n");
        flagError = true;
    }

    return flagError;
}

int write_eeprom(char *message, struct Message *sentMessage, struct Message *receivedMessage)
{
    char batch[20];
    FILE *file = fopen("/opt/gabriel/bin/lote.txt", "r");

    if (file)
    {
        char line[255];
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%s", batch);
        }
    }
    else
    {
        perror("Failed to open lote.txt");
        strcpy(batch, "20230924");
    }

    send_and_receive_message(sentMessage, receivedMessage, message,
                             PROTOCOL_VERSION, MSG_TYPE_SET,
                             MSG_CMD_EEPROM_DISABLE_WRITE_PROTECTION, 0, "True");

    send_and_receive_message(sentMessage, receivedMessage, message,
                             PROTOCOL_VERSION, MSG_TYPE_SET,
                             MSG_CMD_BATCH_NUMBER, 0, batch);

    send_and_receive_message(sentMessage, receivedMessage, message,
                             PROTOCOL_VERSION, MSG_TYPE_SET,
                             MSG_CMD_EEPROM_ENABLE_WRITE_PROTECTION, 0, "True");

    return 0;
}

int test_ports()
{
    int ipsGiven = 0;
    bool flagError = false;

    FILE *file = fopen("/tmp/dhcp.leases", "r");
    if (!file)
    {
        perror("Failed to open dhcp.leases");
        fprintf(logfd, "\t- Failed to open dhcp.leases file (check openWrt image)");
        flagError = true;
        return flagError;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        ipsGiven++;
        char lease_expiry[50], mac_address[50], ip_address[50], hostname[50], client_id[50];

        if (sscanf(line, "%s %s %s %s %s", lease_expiry, mac_address, ip_address, hostname, client_id) == 5)
        {
            if(ping_device(ip_address)){
                log_error(&flagError, "SWITCH ERRORS:\n", "\t- Couldn't ping all devices (check connections and power)\n");
                flagError = true;
            }
        }
    }

    if (ipsGiven != 3)
    {
        log_error(&flagError, "SWITCH ERRORS:\n", "\t- Expected 3 devices\n");
        flagError = true;
    };

    fclose(file);
    return flagError;
}

int test_STM(char *message, struct Message *sentMessage, struct Message *receivedMessage)
{
    bool dummyFlag = false;

    send_and_receive_message(sentMessage, receivedMessage, message,
                             PROTOCOL_VERSION, MSG_TYPE_SET,
                             MSG_CMD_TEST_START, 0, "" /*"True"*/);

    if (serialport == -1)
    {
        printf("Could not open Serial port\n");
        return -1;
    }
    if (receivedMessage->messageType != MSG_TYPE_RESP)
    {
        log_error(&dummyFlag, "STM32 ERROR:\n", "\t- Not expected response from STM32\n");
        return -1;
    }

    send_and_receive_message(sentMessage, receivedMessage, message,
                             PROTOCOL_VERSION, MSG_TYPE_GET,
                             MSG_CMD_TEST_DISPLAY_RESULT, 0, "" /*"True"*/);

    if (strcmp(receivedMessage->messageData, "PASSED"))
    {
        log_error(&dummyFlag, "DISPLAY ERROR:\n", "\t- Display not working\n");
    }

    send_and_receive_message(sentMessage, receivedMessage, message,
                             PROTOCOL_VERSION, MSG_TYPE_GET,
                             MSG_CMD_TEST_EEPROM_RESULT, 0, "" /*"True"*/);

    if (strcmp(receivedMessage->messageData, "PASSED"))
    {
        log_error(&dummyFlag, "EEPROM ERROR:\n", "\t- EEPROM not working\n");
    }

    return 0;
}

int test_modem()
{
    char buffer[255];
    bool flagError = false;

    send_AT_command("AT\r");

    if (receive_AT_command(buffer))
    {
        log_error(&flagError, "MODEM ERRORS:\n", "\t- Modem response not expected\n");
    }

    send_AT_command("AT+CPIN?\r");

    if (receive_AT_command(buffer))
    {
        log_error(&flagError, "MODEM ERRORS:\n", "\t- SIM Card not inserted\n");
    }

    close(serialmodem);

    return flagError;
}

int flash_STM_sequence()
{
    for (int number_of_retries = 0; number_of_retries < 3; number_of_retries++)
    {
        start_flash_sequence();
        if (flash_STM())
        {
            stop_flash_sequence();
        }
        else
        {
            stop_flash_sequence();
            break;
        }
        if (number_of_retries == 2)
        {
            printf("Couldn't flash STM\n");
            return 1;
        }
    }
    return 0;
}

void run_tests(FILE *logfile)
{
    struct Message receivedMessage[6];
    struct Message sentMessage[6];

    bool flagRejectedTest = false;

    get_timestamp(logfile);

    fprintf(logfile, "\n\t------- LOG ERRORS -------\n\n");

    char message[256];

    if (test_STM(message, sentMessage, receivedMessage))
    {
        log_error(&flagRejectedTest, "", "\t- STM32 NOT FUNCTIONING\n");
        flagRejectedTest = true;
    }

    if (test_modem())
    {
        send_and_receive_message(sentMessage, receivedMessage, message,
                                 PROTOCOL_VERSION, MSG_TYPE_SET,
                                 MSG_CMD_TEST_MODEM_RESULT, 0, "False");
        log_error(&flagRejectedTest, "", "\t- MODEM FAILED\n");
        flagRejectedTest = true;
    }
    else
    {
        send_and_receive_message(sentMessage, receivedMessage, message,
                                 PROTOCOL_VERSION, MSG_TYPE_SET,
                                 MSG_CMD_TEST_MODEM_RESULT, 0, "True");
    }

    if (test_eth())
    {
        send_and_receive_message(sentMessage, receivedMessage, message,
                                 PROTOCOL_VERSION, MSG_TYPE_SET,
                                 MSG_CMD_TEST_ETHERNET_RESULT, 0, "False");
        log_error(&flagRejectedTest, "", "\t- ETH FAILED\n");
        flagRejectedTest = true;
    }
    else
    {
        send_and_receive_message(sentMessage, receivedMessage, message,
                                 PROTOCOL_VERSION, MSG_TYPE_SET,
                                 MSG_CMD_TEST_ETHERNET_RESULT, 0, "True");
    }

    if (test_ports())
    {
        send_and_receive_message(sentMessage, receivedMessage, message,
                                 PROTOCOL_VERSION, MSG_TYPE_SET,
                                 MSG_CMD_TEST_SWITCH_RESULT, 0, "False");
        log_error(&flagRejectedTest, "", "\t- PORTs FAILED\n");
        flagRejectedTest = true;
    }
    else
    {
        send_and_receive_message(sentMessage, receivedMessage, message,
                                 PROTOCOL_VERSION, MSG_TYPE_SET,
                                 MSG_CMD_TEST_SWITCH_RESULT, 0, "True");
    }

    write_eeprom(message, sentMessage, receivedMessage);

    if (flagRejectedTest)
    {
        fprintf(logfile, "\n\t******** REJECTED ********\n");
    }
    else
    {
        fprintf(logfile, "\n\t******** APPROVED ********\n");
    }

    fclose(logfile);
    close(serialmodem);
    start_charlesgo();
}