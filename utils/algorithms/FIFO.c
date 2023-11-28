

ExecutedTask *get_fifo_output(process *process_array, int process_array_size, int *executed_tasks_size)
{

    *executed_tasks_size = 0;
    proc_queue *queue = (proc_queue *)malloc(sizeof(proc_queue));
    if (queue == NULL)
    {
        printf("Allocation error");
    }
    create_queue(queue);

    int current_time = 0;
    ExecutedTask *executed_tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    int executed_size = 0;

    process *executed = (process *)malloc(sizeof(process) * process_array_size);

    for (int i = 0; i < process_array_size; i++)
    {
        add_to_queue_fifo(queue, process_array[i]);
    }
    while (!is_execution_done(executed, executed_size, process_array, process_array_size))
    {

        if (!is_queue_empty(queue))
        {
            process execute = remove_from_queue(queue);

            if (!is_in_old_list(execute, executed, executed_size))
            {
                executed[executed_size] = execute;
                executed_size++;
            }

            if (current_time < execute.arrived_at)
                current_time = execute.arrived_at;
            add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, current_time + execute.execution_time, execute.name));
            current_time += execute.execution_time;
        }
    }

    executed_tasks = format_executed_tasks(executed_tasks, executed_tasks_size, process_array, process_array_size);
    free(queue);
    if (executed_tasks != NULL)
        return executed_tasks;
    else
        return NULL;
}
