#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    unsigned long user, nice, system, idle, iowait, irq, softirq;
} CpuSnapshot;

int safe_fgets(char *buffer, size_t size, FILE *fp) {
    if (!fgets(buffer, size, fp)) {
        fprintf(stderr, "fgets failed (EOF or error)\n");
        return 0;
    }
    return 1;
}

int read_cpu_snapshot(CpuSnapshot *snap) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("fopen /proc/stat");
        return -1;
    }

    char line[1024];
    char label[10];

    if (!safe_fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    int parsed = sscanf(line, "%9s %lu %lu %lu %lu %lu %lu %lu",
                        label,
                        &snap->user, &snap->nice, &snap->system,
                        &snap->idle, &snap->iowait, &snap->irq, &snap->softirq);

    // Accept >=8 (label + 7 values), even if more fields exist
    if (parsed < 8) {
        fprintf(stderr, "ERROR: Expected at least 8 fields, got %d\n", parsed);
        return -1;
    }

    return 0;
}


void PrintCpuStats() {
    FILE *statFile = fopen("/proc/stat", "r");
    if (!statFile) {
        perror("Failed to open /proc/stat");
        return;
    }
    
    char line[1024]; // WARNING: Fixed buffer size. Assumes lines < 1024 bytes.
    printf("CPU usage since boot:\n");
    printf("-------------------------------------------------------------\n");
    printf("CPU     | user   | nice   | system | idle   | iowait | irq   | softirq\n");
    printf("-------------------------------------------------------------\n");
    unsigned long user, nice, system, idle, iowait, irq, softirq; 
    char cpuname[20];
    while (safe_fgets(line, sizeof(line), statFile)) {
        if (strncmp(line, "cpu", 3) != 0) {
            break; // Done reading CPU lines
        }
        int parseret = sscanf(line, "%5s %lu %lu %lu %lu %lu %lu %lu", cpuname, &user, &nice, &system, &idle, &iowait, &irq, &softirq);
        printf("%-7s | %6lu | %6lu | %6lu | %6lu | %6lu | %5lu | %7lu \n", cpuname, user, nice, system, idle, iowait, irq, softirq); // TODO: Parse and format fields individually later
    }

    fclose(statFile);
}

int PrintCpuDelta() {
    CpuSnapshot before, after;

    // First snapshot
    if (read_cpu_snapshot(&before) < 0) {
        fprintf(stderr, "Failed to read initial CPU stats.\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        sleep(1);

        // Second snapshot
        if (read_cpu_snapshot(&after) < 0) {
            fprintf(stderr, "Failed to read updated CPU stats.\n");
            return 1;
        }

        // Compute totals
        unsigned long total_before = before.user + before.nice + before.system +
                                     before.idle + before.iowait + before.irq + before.softirq;

        unsigned long total_after = after.user + after.nice + after.system +
                                    after.idle + after.iowait + after.irq + after.softirq;

        unsigned long delta_total = total_after - total_before;
        unsigned long delta_idle = after.idle - before.idle;

        if (delta_total > 0) {
            double usage = 100.0 * (delta_total - delta_idle) / delta_total;
            printf("\rCPU Usage: %.2f%%", usage);
            fflush(stdout);
        }


        // Move 'after' into 'before' for next loop iteration
        before = after;
    }
    printf("\n");
    return 0;
}


void PrintRamStats() {
    FILE *memFile = fopen("/proc/meminfo", "r");
    if (!memFile) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    char line[1024];
    char label[64];
    unsigned long valueKB;

    unsigned long memTotal = 0;
    unsigned long memAvailable = 0;

    while (fgets(line, sizeof(line), memFile)) {
        if (sscanf(line, "%63s %lu", label, &valueKB) == 2) {
            if (strcmp(label, "MemTotal:") == 0) {
                memTotal = valueKB;
            } else if (strcmp(label, "MemAvailable:") == 0) {
                memAvailable = valueKB;
            }

            if (memTotal && memAvailable) {
                break; // Got what we need
            }
        }
    }

    fclose(memFile);

    printf("\nRAM usage (in MB):\n");
    printf("------------------\n");
    printf("Total:      %4lu MB\n", memTotal / 1024);
    printf("Available:  %4lu MB\n", memAvailable / 1024);
    printf("Used:       %4lu MB\n", (memTotal - memAvailable) / 1024);
}

int main() {
    PrintCpuStats();
    PrintRamStats();
    int check = PrintCpuDelta();
    return 0;
}
