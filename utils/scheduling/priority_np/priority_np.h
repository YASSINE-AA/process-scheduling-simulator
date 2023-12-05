#ifndef PRIORITY_NP_H
#define PRIORITY_NP_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../../queues/priority/priority_queue.h"
#include "../../../include/types.h"
#include "../useful/useful.h"
#include "../../gantt/format.h"

ExecutedTask *get_priority_np_output(process *process_array, int process_array_size, int *executed_tasks_size);
#endif