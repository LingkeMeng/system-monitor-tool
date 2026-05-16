///_|> descry: gets the number of available CPU cores
///_|> returning: the number of cores, type int
int get_cpu_cores();

///_|> descry: draws the grid representing each CPU core
///_|> rows: number of rows of cores in display, type int
///_|> cols: number of columns, type int
///_|> start_row: the row to begin drawing from, type int
///_|> returning: this function does not return anything
void draw_cores(int rows, int cols, int start_row);