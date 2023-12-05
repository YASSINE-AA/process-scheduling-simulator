#ifndef PRIORITY_H
#define PRIORITY_H

#include "../../queues/priority/priority_queue.h"
#include "../../../include/types.h"

ExecutedTask *get_priority_output(process *process_array, int process_array_size, int *executed_tasks_size);
#endif