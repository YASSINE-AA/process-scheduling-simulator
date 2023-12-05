#ifndef USEFUL_H
#define USEFUL_H

#include <stdbool.h>
#include <stddef.h>
#include "../../../include/types.h"

bool is_in_old_list(process p, process *old_process_list, int old_process_list_size);
int get_earliest_time(process *process_array, int process_array_size);
void sort_process_array_by_at(process *process_array, int process_array_size);
void free_process_array(process *arr);


#endif