#ifndef USEFUL_H
#define USEFUL_H

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../useful/useful.h"
#include "../../gantt/format.h"

bool is_in_old_list(process p, process *old_process_list, int old_process_list_size);
int get_earliest_time(process *process_array, int process_array_size);
void sort_process_array_by_at(process *process_array, int process_array_size);
void free_process_array(process *arr);


#endif