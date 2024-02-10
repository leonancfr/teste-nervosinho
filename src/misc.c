#include "misc.h"

void get_timestamp(FILE *logfile){
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    char timestamp[20];  // Sufficient space for "YYYY-MM-DD HH:MM:SS" and null terminator
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(logfile, "\n Teste realizado em: %s\n", timestamp);
}

int start_charlesgo(){
    return system("service charles_go start");
}

int stop_charlesgo(){
    return system("service charles_go stop");
}

int start_flash_sequence(){
    return system("echo \"default-on\" > /sys/class/leds/BOOT_STM32/trigger && sleep 1 && echo \"none\" > /sys/class/leds/RESET_STM32/trigger && sleep 1 && echo \"default-on\" > /sys/class/leds/RESET_STM32/trigger && sleep 1");
}

int stop_flash_sequence(){
    return system("echo \"none\" > /sys/class/leds/BOOT_STM32/trigger && sleep 1 && echo \"none\" > /sys/class/leds/RESET_STM32/trigger && sleep 1 && echo \"default-on\" > /sys/class/leds/RESET_STM32/trigger");
}

int flash_STM(){
    return system("stm32flash -v -w /opt/gabriel/bin/firmware_stm32.bin -b 115200 /dev/ttyS1");
}

int get_batch_file(){
    return system("wget https://gitlab.com/-/snippets/3634708/raw -O /opt/gabriel/bin/lote.txt");
}

int run_eth_ping(){
    return system("ping -c 4 baidu.com");
}

int os_run_command(char* command){
    return system(command);
}