#include "types.h"
#include "useful.c"

void round_robin_output(process* process_array, int process_array_size) {
    // see who arrived first
    int first_indx = who_arrived_first(process_array, process_array_size);
    
}