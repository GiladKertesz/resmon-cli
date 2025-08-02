#ifndef ARGS_H
#define ARGS_H

typedef struct {
    int loop;
    int show_cpu_stat;
    int show_ram;
    int show_cpu_usage;
    int interval;
} Config;

void parse_args(int argc, char *argv[], Config *config);
#endif
