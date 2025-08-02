#ifndef READ_UTILS_H
#define READ_UTILS_H

typedef struct {
    unsigned long user, nice, system, idle, iowait, irq, softirq;
} CpuSnapshot;


void PrintCpuStats();
int PrintCpuDelta();
void PrintRamStats();

#endif
