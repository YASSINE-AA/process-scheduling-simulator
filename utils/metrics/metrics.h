#ifndef METRICS_H
#define METRICS_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../../include/types.h"

int get_rotation_time(const char *process_name, ExecutedTask *executed, int executed_size);
int get_waiting_time(const char *process_name, ExecutedTask *executed, int executed_size, process *process_array, int process_array_size);

#endif