ExecutedTask *get_priority_output(process *process_array, int process_array_size, int *executed_tasks_size)
{

    *executed_tasks_size = 0;
    priority_queue *queue = init_priority_queue(1000);
    if (queue == NULL)
    {
        printf("Allocation error");
    }
    int current_time = 0;
    ExecutedTask *executed_tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    int executed_size = 0;
    process *executed = (process *)malloc(sizeof(process) * process_array_size);
    sort_process_array_by_at(process_array, process_array_size);
    while (!is_execution_done(executed, executed_size, process_array, process_array_size))
    {
 
        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at == current_time)
            {
                add_to_pr_queue_p(queue, process_array[i]);
            }
        }

        if (!is_pr_queue_empty(queue))
        {
            process execute = remove_from_pr_queue_p(queue);

            execute.execution_time--;
            printf("%d %s %d\n", current_time, execute.name, execute.execution_time);
            if (execute.execution_time > 0)
            {
                add_to_pr_queue_p(queue, execute);
            }
            else
            {
                if (!is_in_old_list(execute, executed, executed_size))
                {
                    executed[executed_size] = execute;
                    executed_size++;
                }
            }

            add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, current_time + 1, execute.name));
            current_time++;
        }
        else
        {
            current_time++;
        }
    }

    executed_tasks = format_executed_tasks(executed_tasks, executed_tasks_size, process_array, process_array_size);
    free_priority_queue(queue);
    if (executed_tasks != NULL)
        return executed_tasks;
    return NULL;
}
