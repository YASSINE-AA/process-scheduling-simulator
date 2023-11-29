
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
