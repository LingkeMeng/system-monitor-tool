#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <getopt.h>

#include <limits.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include "memory.h"

int ceil_custom(double value) {
    int int_value = (int)value;
    return (value > int_value) ? (int_value + 1) : int_value;
}

void update_memory_plot(double current_mem, int index, double total_mem) {
    int row_offset = 4;
    printf("\033[3;1H");  
    printf("v Memory    %.2f GB        ", current_mem);

    int height = (int)((current_mem / total_mem) * 12);  
    int row = 12 - height + row_offset; 
    printf("\033[%d;%dH#", row, 11 + index);  

    fflush(stdout);
}

void get_memory_info(double *total_gb, double *used_gb) {
    FILE *fp = fopen("/proc/meminfo", "r");
    long mem_total = 0, mem_available = 0;
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        if (mem_total == 0)
            sscanf(line, "MemTotal: %lu kB", &mem_total);
        if (mem_available == 0)
            sscanf(line, "MemAvailable: %lu kB", &mem_available);
        if (mem_total > 0 && mem_available > 0)
            break;
    }
    fclose(fp);
    //ceil
    *total_gb = ceil_custom((mem_total * 1024.0) / (1024 * 1024 * 1024));  
    *used_gb = ((mem_total - mem_available) * 1024.0) / (1024 * 1024 * 1024);
}

void draw_memory_static_frame(double total_num, int sample_num) {
    int base_row = 4; 

    for (int i = 12; i >= 0; i--) {
        int row = base_row + (12 - i);
        printf("\033[%d;1H", row);

        if (i == 12) {
            printf(" %d GB   |", (int)total_num);
        } else if (i == 0) {
            printf("  0 GB   ");
        } else {
            printf("         |");
        }
    }

    // int x_axis_row = base_row + 13;
    // printf("\033[%d;9H", x_axis_row);  
    for (int i = 0; i < sample_num / 2; i++) {
        printf("——"); 
    }
    fflush(stdout);
}
