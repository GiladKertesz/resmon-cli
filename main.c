#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "read_utils.h"
#include "args.h"

void clear_screen() {
    printf("\033[H\033[J");
}

void call_funcs(Config config) {
    if (config.show_cpu_stat) {
            PrintCpuStats();
        }
    if (config.show_ram) {
            PrintRamStats();
        }
    if (config.show_cpu_usage) {
            PrintCpuDelta();
    }

}


int main(int argc, char *argv[]) {
    Config config;
    parse_args(argc, argv, &config);
    while (config.loop){
        clear_screen();
        call_funcs(config);    
        sleep(config.interval);
    }
    call_funcs(config);
    return 1;
}