#include <stdio.h>


void ShowInitialStat(){
    //read /proc/stat
    FILE *statfileptr = fopen("/proc/stat", "r");
    
    //read first line
    char line[1024]; // WARNING: Fixed buffer size. Assumes lines are <1024 bytes. Consider dynamic allocation later.
    fgets(line, sizeof(line), statfileptr);

    printf("CPU usage since boot:\n");
    printf("CPU | user | nice | system | idle | iowait | irq | softirq\n");
    while(line[0]=='c'){
        //print each line of CPU usage
        printf("%s", line);
        fgets(line, sizeof(line), statfileptr);
    }
    fclose(statfileptr);

    FILE *memfileptr = fopen("/proc/meminfo", "r");
    fgets(line, sizeof(line), memfileptr);
    printf("\nRAM usage\n");
    int ramuse;
    char ramtype[100]; // WARNING: Fixed buffer size. Assumes lines are <100 bytes. Consider dynamic allocation later.
    while(fgets(line, sizeof(line), memfileptr)) {
        if(line[0]=='S'){
            break;
        } else {
            int sscanf_res = sscanf(line, "%s %d", ramtype, &ramuse);
            printf("%s %d MB\n", ramtype, ramuse/1024);
        }
    }
    fclose(memfileptr);


}

int main () {
    ShowInitialStat();
    return 0;
}
