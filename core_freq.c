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
#include "core_freq.h"

float get_max_frequency() {
    FILE *fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    if (fp == NULL) {
        return -1;
    }

    long khz;
    if (fscanf(fp, "%ld", &khz) != 1) {
        fclose(fp);
        return -1.0;
    }

    fclose(fp);
    return khz / 1000000.0;
}
