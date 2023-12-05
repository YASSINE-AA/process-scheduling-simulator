#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../../../include/types.h"

typedef struct proc_in_queue
{
    process value;
    struct proc_in_queue *next;
} proc_in_queue;

typedef struct
{
    proc_in_queue *head;
    proc_in_queue *tail;
} proc_queue;

bool is_queue_empty(proc_queue *q);
void create_queue(proc_queue *q);
void add_to_queue(proc_queue *q, process proc);
void add_to_queue_fifo(proc_queue *q, process proc);
process remove_from_queue(proc_queue *q);

#endif 