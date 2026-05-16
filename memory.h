///_|> descry: gets total and used memory from /proc/meminfo
///_|> total_gb: pointer to store total memory (GB), type double*
///_|> used_gb: pointer to store used memory (GB), type double*
///_|> returning: this function does not return anything
void get_memory_info(double *total_gb, double *used_gb);

///_|> descry: updates the memory usage plot with new data
///_|> current_mem: current used memory in GB, type double
///_|> index: horizontal position in plot, type int
///_|> total_mem: total memory in GB, type double
///_|> returning: this function does not return anything
void update_memory_plot(double current_mem, int index, double total_mem);

///_|> descry: rounds a double value up to the nearest int
///_|> value: floating-point number to round, type double
///_|> returning: the rounded-up integer value, type int
int ceil_custom(double value);

///_|> descry: draws the memory usage frame
///_|> total_num: total memory in GB, type double
///_|> sample_num: number of samples to display, type int
///_|> returning: this function does not return anything
void draw_memory_static_frame(double total_num, int sample_num);