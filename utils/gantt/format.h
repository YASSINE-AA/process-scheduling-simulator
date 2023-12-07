#ifndef FORMAT_H
#define FORMAT_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/types.h"
#include <string.h>
#include <time.h>

ExecutedTask get_task(int start, int finish, int arrival_time, char *label);
void add_to_executed_tasks(ExecutedTask *executed_tasks, int *executed_tasks_size, ExecutedTask task);
ExecutedTask *format_executed_tasks(ExecutedTask *tasks, int *tasks_size, process *process_array, int process_array_size);

#endif