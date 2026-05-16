
typedef struct {
    unsigned long user, nice, system, idle, iowait, irq, softirq, steal;
} CPUstats;

///_|> descry: this function updates the CPU usage plot based on current usage
///_|> current_cpu: the current CPU usage percentage, type double
///_|> index: the horizontal position in the plot, type int
///_|> has_memory: whether memory is being displayed (affects layout), type int
///_|> returning: nothing
void update_cpu_plot(double current_cpu, int index, int has_memory);

///_|> descry: reads current CPU stats from /proc/stat and fills the given struct
///_|> stats: pointer to CPUstats structure to populate, type CPUstats*
///_|> returning: this function does not return anything
void get_cpu_stats(CPUstats *stats);

///_|> descry: calculates CPU usage between two moments in time
///_|> prev: previous CPU statistics, type CPUstats
///_|> curr: current CPU statistics, type CPUstats
///_|> returning: calculated CPU usage as a percentage, type double
double calculate_cpu_usage(CPUstats prev, CPUstats curr);

///_|> descry: draws the frame to display CPU usage
///_|> sample_num: number of data points (width of graph), type int
///_|> has_memory: whether memory is being shown (affects positioning), type int
///_|> returning: this function does not return anything
void draw_cpu_static_frame(int sample_num, int has_memory);