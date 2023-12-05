#include "priority_queue.h"
priority_queue *init_priority_queue()
{
    priority_queue *pq = (priority_queue *)malloc(sizeof(priority_queue));
    pq->front = NULL;
    return pq;
}

void print_queue(priority_queue *pq, int current_time)
{
    Node *current = pq->front;
    printf("\n===========%d============\n", current_time);
    while (current != NULL)
    {
        printf("%s %d\n", current->process.name, current->process.execution_time);
        current = current->next;
    }
    printf("\n========================\n");
}

void swap_exec_time(priority_queue *pq, process p)
{
    Node *current = pq->front;
    while (current != NULL)
    {
        if (strcmp(current->process.name, p.name) == 0)
        {
            current->process.execution_time = p.execution_time;
            break;
        }
        current = current->next;
    }
}

process get_front(priority_queue *pq)
{
    process invalid_process = {-1, -1, -1, ""};
    if (pq->front != NULL)
    {
        return pq->front->process;
    }
    return invalid_process;
}

void add_to_pr_queue(priority_queue *pq, process new_process)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->process = new_process;
    new_node->next = NULL;

    if (pq->front == NULL || pq->front->process.execution_time > new_process.execution_time)
    {

        new_node->next = pq->front;
        pq->front = new_node;
    }
    else
    {
        Node *current = pq->front;
        Node *previous = NULL;

        while (current != NULL && current->process.execution_time <= new_process.execution_time)
        {
            previous = current;
            current = current->next;
        }

        if (current == NULL)
        {
            previous->next = new_node;
        }
        else
        {
            new_node->next = current;
            if (previous != NULL)
                previous->next = new_node;
            else
                pq->front = new_node;
        }
    }
}

process remove_from_pr_queue(priority_queue *pq)
{
    if (pq->front == NULL)
    {
        process empty_process = {0, 0, 0, ""};
        return empty_process;
    }

    process removed_process = pq->front->process;
    Node *temp = pq->front;
    pq->front = pq->front->next;
    free(temp);

    return removed_process;
}

bool is_in_queue(priority_queue *pq, char *process_name)
{
    bool val = false;
    if (pq->front != NULL)
    {
        Node *current = pq->front;
        while (current->next != NULL)
        {
            if (strcmp(current->process.name, process_name) == 0)
            {
                val = true;
                break;
            }
            current = current->next;
        }
        return val;
    }
    return false;
}

void add_to_pr_queue_p(priority_queue *pq, process new_process)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->process = new_process;
    new_node->next = NULL;

    if (pq->front == NULL || pq->front->process.priority > new_process.priority)
    {

        new_node->next = pq->front;
        pq->front = new_node;
    }
    else
    {
        Node *current = pq->front;
        Node *previous = NULL;

        while (current != NULL && current->process.execution_time <= new_process.priority)
        {
            previous = current;
            current = current->next;
        }

        if (current == NULL)
        {
            previous->next = new_node;
        }
        else
        {
            new_node->next = current;
            if (previous != NULL)
                previous->next = new_node;
            else
                pq->front = new_node;
        }
    }
}

process remove_from_pr_queue_p(priority_queue *pq)
{
    if (pq->front == NULL)
    {
        process empty_process = {0, 0, 0, ""};
        return empty_process;
    }

    process removed_process = pq->front->process;
    Node *temp = pq->front;
    pq->front = pq->front->next;
    free(temp);

    return removed_process;
}

int is_pr_queue_empty(priority_queue *pq)
{
    return pq->front == NULL;
}

void free_priority_queue(priority_queue *pq)
{
    free(pq);
}