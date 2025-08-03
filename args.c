#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "args.h"

void parse_args(int argc, char *argv[], Config *config) {
    config->loop = 0;
    config->show_cpu_stat = 0;
    config->show_ram = 0;
    config->show_cpu_usage = 0;
    config->interval = 1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--once") == 0) {
            config->loop = 0;
        } else if (strcmp(argv[i], "--loop") == 0) {
            config->loop = 1;
        } else if (strcmp(argv[i], "--cpustat") == 0) {
            config->show_cpu_stat = 1;
        } else if(strcmp(argv[i], "--cpusage") == 0) {
            config->show_cpu_usage = 1;
        } else if (strcmp(argv[i], "--ram") == 0) {
            config->show_ram = 1;
        } else if (strcmp(argv[i], "--interval") == 0 && i + 1 < argc) {
            config->interval = atoi(argv[++i]);
            if (config->interval <= 0) {
                fprintf(stderr, "Invalid interval: %s\n", argv[i]);
                exit(1);
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: resmon-cli [options]\n");
            printf("Options:\n");
            printf("  --cpu            Show CPU statistics\n");
            printf("  --ram            Show RAM statistics\n");
            printf("  --loop           Continuously refresh output\n");
            printf("  --once           Run only once (default)\n");
            printf("  --interval <n>   Set refresh interval in seconds (default: 1)\n");
            printf("  --help, -h       Show this help message and exit\n");
            exit(0);
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            exit(1);
        }
    }

    if (!config->show_cpu_stat && !config->show_ram && !config->show_cpu_usage) {
        config->show_cpu_stat = 1;
        config->show_ram = 1;
        config->show_cpu_usage = 1;
    }
}
