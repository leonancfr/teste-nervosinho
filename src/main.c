#include "communication.h"
#include "testfunctions.h"
#include "config.h"
#include "config_serial.h"

FILE *logfile = NULL;

void init(){
    char *STMSerialPath = "/dev/ttyS1";
    char *modemSerialPath = "/dev/ttyUSB2";
    logfile = fopen("/root/logHardwareTest.txt", "a");

    stop_charlesgo();
    flash_STM_sequence();

    os_run_command("sleep 30");

    communication_init(logfile, STMSerialPath, modemSerialPath);
}

int main(void)
{
    init();
    run_tests(logfile);
    return 0;
}




