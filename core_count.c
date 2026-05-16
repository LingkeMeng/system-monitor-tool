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
#include "core_count.h"

int get_cpu_cores() {
    return sysconf(_SC_NPROCESSORS_ONLN); 
}

void draw_cores(int rows, int cols, int start_row) {
    for (int i = 0; i < rows; i++) {
        int row_base = start_row + i * 3;

        printf("\033[%d;1H", row_base);
        for (int j = 0; j < cols; j++) {
            printf("+---+    ");
        }
        // 
        printf("\033[%d;1H", row_base + 1);
        for (int j = 0; j < cols; j++) {
            printf("|   |    ");
        }
        // +---+
        printf("\033[%d;1H", row_base + 2);
        for (int j = 0; j < cols; j++) {
            printf("+---+    ");
        }
    }

    fflush(stdout);
}
