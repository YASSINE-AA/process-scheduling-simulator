bool is_queue_empty(proc_queue *q)
{
    return q->head == NULL;
}

void create_queue(proc_queue *q)
{
    q->head = NULL;
    q->tail = NULL;
}

void add_to_queue(proc_queue *q, process proc)
{
    proc_in_queue *new_process = (proc_in_queue *)malloc(sizeof(proc_in_queue));
    if (new_process == NULL)
    {

        return;
    }
    new_process->value = proc;
    new_process->next = NULL;

    if (is_queue_empty(q))
    {
        q->head = new_process;
        q->tail = new_process;
    }
    else
    {
        q->tail->next = new_process;
        q->tail = new_process;
    }
}

void add_to_queue_fifo(proc_queue *q, process proc)
{
    proc_in_queue *new_process = (proc_in_queue *)malloc(sizeof(proc_in_queue));
    if (new_process == NULL)
    {
        return;
    }
    new_process->value = proc;
    new_process->next = NULL;

    if (is_queue_empty(q))
    {
        q->head = new_process;
        q->tail = new_process;
    }
    else
    {
        if (proc.arrived_at < q->head->value.arrived_at)
        {
            new_process->next = q->head;
            q->head = new_process;
        }
        else
        {
            proc_in_queue *current = q->head;
            proc_in_queue *prev = NULL;

            while (current != NULL && proc.arrived_at >= current->value.arrived_at)
            {
                prev = current;
                current = current->next;
            }

            prev->next = new_process;
            new_process->next = current;

            if (current == NULL)
            {
                q->tail = new_process;
            }
        }
    }
}

process remove_from_queue(proc_queue *q)
{

    process empty_process;
    empty_process.name[0] = '\0';
    empty_process.arrived_at = 0;
    empty_process.execution_time = -1;

    if (!is_queue_empty(q))
    {
        proc_in_queue *proc_to_delete = q->head;
        q->head = q->head->next;
        process result = proc_to_delete->value;
        free(proc_to_delete);
        return result;
    }

    return empty_process;
}

priority_queue *init_priority_queue()
{
    priority_queue *pq = (priority_queue *)malloc(sizeof(priority_queue));
    pq->front = NULL;
    return pq;
}

void print_queue(priority_queue *pq)
{
    Node *current = pq->front;
    while (current != NULL)
    {
        printf("%s\n", current->process.name);
        current = current->next;
    }
    printf("\n========================\n");
}
#include <string.h>

process get_next_proc(priority_queue *pq, process reference)
{
    process invalid_process = (process){-1, -1, -1, ""};
    Node *current = pq->front;

    while (current != NULL)
    {
        if (strcmp(current->process.name, reference.name) == 0)
        {
            if (current->next != NULL)
                return current->next->process;
            else
                return invalid_process;
        }
        current = current->next;
    }

    return invalid_process;
}


int get_at_process_with_higher_pr(priority_queue *pq, process reference)
{
    bool found = false;
    Node *current = pq->front;

    int result = -1;

    while (current != NULL)
    {
        if (current->process.priority > reference.priority)
        {
            found = true;
            result = current->process.arrived_at;
            break;
        }
        current = current->next;
    }

    return result;
}
void add_to_pr_queue(priority_queue *pq, process new_process)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->process = new_process;
    new_node->next = NULL;

    if (pq->front == NULL || pq->front->process.execution_time >= new_process.execution_time)
    {
        new_node->next = pq->front;
        pq->front = new_node;
    }
    else
    {
        Node *current = pq->front;
        while (current->next != NULL && current->next->process.execution_time < new_process.execution_time)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
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

        while (current->next != NULL && current->next->process.priority < new_process.priority)
        {
            current = current->next;
        }
        if (current->next != NULL && current->next->process.priority == new_process.priority)
        {
            new_process.ignore_equal = true;
            current->next->next = new_node;
            if (current->next->next->next != NULL)
                new_node->next = current->next->next->next;
        }
        else
        {
            new_node->next = current->next;
            current->next = new_node;
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
