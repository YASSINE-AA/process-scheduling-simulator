#ifndef SRT_H
#define SRT_H

#include "../../queues/priority/priority_queue.h"
#include "../../../include/types.h"
#include "../useful/useful.h"
#include "../../gantt/format.h"

ExecutedTask *get_srt_output(process *process_array, int process_array_size, int *executed_tasks_size);
#endif