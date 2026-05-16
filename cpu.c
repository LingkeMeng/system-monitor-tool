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
#include "cpu.h"

void update_cpu_plot(double current_cpu, int index, int has_memory) {
    int row_offset = has_memory ? 19 : 4;
    int col_offset = 10;
    int graph_height = 10;

    // cpu up
    printf("\033[%d;1H", row_offset - 1);
    printf("v CPU  %.2f%%            \n", current_cpu);

    int height = (int)((current_cpu / 100.0) * graph_height);
    //if (height >= graph_height) height = graph_height - 1; 
    int row = row_offset + graph_height - height; // - 1

    printf("\033[%d;%dH:", row, col_offset + index);

    fflush(stdout);
}

void get_cpu_stats(CPUstats *stats) {
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Error opening /proc/stat");
        exit(EXIT_FAILURE);
    }

    if (fscanf(fp, "cpu %lu %lu %lu %lu %lu %lu %lu %lu", 
               &stats->user, &stats->nice, &stats->system, &stats->idle, 
               &stats->iowait, &stats->irq, &stats->softirq, &stats->steal) != 8) {
        perror("Error reading CPU stats");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
}

double calculate_cpu_usage(CPUstats prev, CPUstats curr) {
    unsigned long prev_idle = prev.idle + prev.iowait;
    unsigned long curr_idle = curr.idle + curr.iowait;

    unsigned long prev_total = prev.user + prev.nice + prev.system + prev.idle +
                               prev.iowait + prev.irq + prev.softirq + prev.steal;
    unsigned long curr_total = curr.user + curr.nice + curr.system + curr.idle +
                               curr.iowait + curr.irq + curr.softirq + curr.steal;

    unsigned long total_diff = curr_total - prev_total;
    unsigned long idle_diff = curr_idle - prev_idle;

    if (total_diff == 0) {
        return 0.0;
    }
    return (100.0 * (1.0 - ((double)idle_diff / total_diff)));
}


void draw_cpu_static_frame(int sample_num, int has_memory) {
    int row_offset = has_memory ? 19 : 4; 

    for (int i = 10; i >= 0; i--) {
        int row = row_offset + (10 - i);
        printf("\033[%d;1H", row); 
        if (i == 10) {
            printf("  100%%  |");
        } else if (i == 0) {
            printf("  0%%    ");
        } else {
            printf("        |");
        }
    }

    // int axis_row = row_offset + 11;
    // printf("\033[%d;1H", axis_row);
    // printf("        |");
    for (int i = 0; i < (sample_num + 1) / 2; i++) {
        printf("——");
    }
    //printf("\n");
}
