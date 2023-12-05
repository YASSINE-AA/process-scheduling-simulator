
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../../include/types.h"

typedef struct Node
{
    process process;
    struct Node *next;
} Node;

typedef struct
{
    Node *front;
} priority_queue;

priority_queue *init_priority_queue();
void print_queue(priority_queue *pq, int current_time);
void swap_exec_time(priority_queue *pq, process p);
process get_front(priority_queue *pq);
void add_to_pr_queue(priority_queue *pq, process new_process);
process remove_from_pr_queue(priority_queue *pq);
bool is_in_queue(priority_queue *pq, char *process_name);
void add_to_pr_queue_p(priority_queue *pq, process new_process);
process remove_from_pr_queue_p(priority_queue *pq);
int is_pr_queue_empty(priority_queue *pq);
void free_priority_queue(priority_queue *pq);

#endif