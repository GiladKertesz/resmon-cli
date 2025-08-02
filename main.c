#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "read_utils.h"

typedef struct {
    int loop;
    int show_cpu_stat;
    int show_ram;
    int show_cpu_usage;
} Config;

Config config = {
        .loop = 0,
    .show_cpu_stat = 1,
    .show_ram = 1,
    .show_cpu_usage = 1
}; // default values

void clear_screen() {
    printf("\033[H\033[J");
}


int main() {
    while (config.loop){
        clear_screen();
        if (config.show_cpu_stat) {
            PrintCpuStats();
        }
        if (config.show_ram) {
            PrintRamStats();
        }
        if (config.show_cpu_usage) {
            PrintCpuDelta();
        }
        
        sleep(1);
    }
    if (config.show_cpu_stat) {
        PrintCpuStats();
    }
    if (config.show_ram) {
        PrintRamStats();
    }
    if (config.show_cpu_usage) {
        PrintCpuDelta();
    }
    return 1;
}