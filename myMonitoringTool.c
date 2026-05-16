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
#include "cpu.h"
#include "core_freq.h"
#include "core_count.h"
#include "Config.h"
static pid_t pids[4]; //4 process

void handle_sigint(int sig) {
    char response;
    printf("\nDo you really want to quit? (y/n): ");
    // pause
    for (int i = 0; i < 4; i++) {
        kill(pids[i], SIGSTOP);
    }
    
    fflush(stdout);
    response = getchar();

    if (response == 'y' || response == 'Y') {
        for (int i = 0; i < 4; i++) {
            kill(pids[i], SIGKILL);
            waitpid(pids[i], NULL, 0);
            
        }
        // self kill
        kill(getpid(), SIGTERM);

        exit(0);
    } else {
        for (int i = 0; i < 4; i++) {
            kill(pids[i], SIGCONT);
        }
    }
}

void clear_screen() {
    printf("\033[2J");  // Clear the screen
    printf("\033[H");   // Move cursor to top-left
    fflush(stdout);
}

void handle_sigtstp(int sig) {
    printf("\nCtrl-Z is disabled for this program.\n");
}

int is_positive_integer(const char *str) {
    if (str == NULL || *str == '\0') return 0;  
    while (*str) {
        if (!isdigit(*str)) return 0; 
        str++;
    }
    return 1; 
}

int safe_atoi(const char *str) {
    if (!is_positive_integer(str)) {
        fprintf(stderr, "Error: '%s' is not a valid positive integer.\n", str);
        exit(EXIT_FAILURE);
    }

    long value = strtol(str, NULL, 10);
    if (value <= 0 || value > INT_MAX) {
        fprintf(stderr, "Error: '%s' is out of range. Must be a positive integer within [1, %d].\n", str, INT_MAX);
        exit(EXIT_FAILURE);
    }

    return (int)value;
}

Config parse_args(int argc, char *argv[]) {
    Config Config = {20, 500000, 0, 0, 0};  // Default Config

    int positional_args = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (positional_args == 0) {
                Config.samples = safe_atoi(argv[i]);
                positional_args++;
            } else if (positional_args == 1) {
                Config.tdelay = safe_atoi(argv[i]);
                positional_args++;
            }
        } else {
            break;
        }
    }

    static struct option long_options[] = {
        {"memory", no_argument, 0, 'm'},
        {"cpu", no_argument, 0, 'c'},
        {"cores", no_argument, 0, 'o'},
        {"samples", required_argument, 0, 's'},
        {"tdelay", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "ms:t:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'm': Config.show_memory = 1; break;
            case 'c': Config.show_cpu = 1; break;
            case 'o': Config.show_cores = 1; break;
            case 's': Config.samples = safe_atoi(optarg); break;
            case 't': Config.tdelay = safe_atoi(optarg); break;
            default:
                fprintf(stderr, "Error: Unknown option or incorrect format.\n");
                exit(EXIT_FAILURE);
        }
    }

    if (!Config.show_memory && !Config.show_cpu && !Config.show_cores) {
        Config.show_memory = Config.show_cpu = Config.show_cores = 1;
    }

    return Config;
}

int core_start(Config Config) {
    if (Config.show_memory && !Config.show_cpu) {
        return 19;
    } else if (Config.show_cpu && Config.show_memory) {
        return 32;
    } else if (!Config.show_memory && Config.show_cpu){
        return 16;
    } else return 3;
}


int main(int argc, char *argv[]) {
    Config Config = parse_args(argc, argv);  // Config
    //initialize display
    clear_screen();
    printf("\033[1;1H");
    printf("Nbr of samples: %d -- every %d microSecs (%.3f secs)", Config.samples, Config.tdelay, (double)Config.tdelay / 100000000.0);

    //memory
    double *memory_values = malloc(Config.samples * sizeof(double));
    double total_mem, used_gb;

    //cpu
    double *cpu_values = malloc(Config.samples * sizeof(double));
    CPUstats prev_stats, curr_stats;
    //get_cpu_stats(&cpu_values);

    int pipes[4][2];
   //static pid_t pids[4];
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    for (int k = 0; k < 4; k++) {
        pipe(pipes[k]);
        pids[k] = fork();

        if (pids[k] == 0) {
            signal(SIGINT, SIG_IGN);
            close(pipes[k][0]);

            if (k == 0 && Config.show_memory) {
                //get_memory_info(&total_mem, &used_gb);
                // draw_memory_static_frame(total_mem, Config.samples);
                for (int i = 0; i < Config.samples; i++) {
                    usleep(Config.tdelay);
                    get_memory_info(&total_mem, &used_gb);
                    write(pipes[0][1], &used_gb, sizeof(double));
                    //
                    // memory_values[i] = used_gb;
                    // update_memory_plot(used_gb, i, total_mem);
                }

            } else if (k == 1 && Config.show_cpu) {
                //draw_cpu_static_frame(Config.samples, Config.show_memory);
                for (int i = 0; i < Config.samples; i++) {
                    usleep(Config.tdelay);
                    prev_stats = curr_stats;
                    get_cpu_stats(&curr_stats);
                    double usage = calculate_cpu_usage(prev_stats, curr_stats);
                    // update_cpu_plot(cpu_values[i], i, Config.show_memory);
                    write(pipes[1][1], &usage, sizeof(double));

                }

            } else if (k == 2 && Config.show_cores) {
                //int x = core_start(Config);
                //printf("\033[%d;1H", x);
                int cores_input = get_cpu_cores();
                write(pipes[2][1], &cores_input, sizeof(int));
                //printf("v Number of Cores: %d", cores);
            }

            else if (k == 3 && Config.show_cores) {
                float max_frequency_input = get_max_frequency();
                write(pipes[3][1], &max_frequency_input, sizeof(float));
                //int x = core_start(Config) + 1;
                // printf("\033[%d;1H", x);  // 打印在 cores 信息下面
                // printf("             @ %.2f GHz\n", max_frequency);

                //int cores = get_cpu_cores();
                // int rows = (cores + 3) / 4;
                // int cols = cores < 4 ? cores : 4;
                // draw_cores(rows, cols);
            }
            close(pipes[k][1]);
            exit(0);
            
        }
        
    }
    
    if (Config.show_memory) {
        get_memory_info(&total_mem, &(double){0});
        draw_memory_static_frame(total_mem, Config.samples);
    }
    if (Config.show_cpu) {
        draw_cpu_static_frame(Config.samples, Config.show_memory);
    }
    for (int i = 0; i < Config.samples; i++) {
        double mem_val, cpu_val;
        if (Config.show_memory) {
            read(pipes[0][0], &mem_val, sizeof(double));
            update_memory_plot(mem_val, i, total_mem);
        }
        if (Config.show_cpu) {
            read(pipes[1][0], &cpu_val, sizeof(double));
            update_cpu_plot(cpu_val, i, Config.show_memory);
        }
        
    }
    if (Config.show_cores) {
        int cores;
        float max_frequency;
        int x = core_start(Config);
        printf("\033[%d;1H", x);
        read(pipes[3][0], &max_frequency, sizeof(float));
        read(pipes[2][0], &cores, sizeof(int));
        printf("v Number of Cores: %d @ %.2f GHz\n", cores, max_frequency);

        x=x+1; //next line
        int rows = (cores + 3) / 4;
        int cols = cores < 4 ? cores : 4;
        draw_cores(rows, cols, x);
    }

    for (int k = 0; k < 4; k++) waitpid(pids[k], NULL, 0);
    free(memory_values);
    free(cpu_values);

    return 0;
} 