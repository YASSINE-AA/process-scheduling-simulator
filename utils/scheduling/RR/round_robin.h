#ifndef RR_H
#define RR_H

#include "../../queues/fifo/queue.h"
#include "../../../include/types.h"
#include "../useful/useful.h"

ExecutedTask *get_round_robin_output(int quantum, process *process_array, int process_array_size, int *tasks_size);
#endif