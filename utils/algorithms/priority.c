
ExecutedTask *get_priority_output(process *process_array, int process_array_size, int *executed_tasks_size)
{
    // preemptive
    int finish_track_arr_size = 0;
    finish_track *finish_track_arr = (finish_track *)malloc(sizeof(finish_track) * process_array_size);
    if (finish_track_arr == NULL)
        return NULL;

    *executed_tasks_size = 0;
    priority_queue *queue = init_priority_queue(1000);
    if (queue == NULL)
        return NULL;

    int current_time = 0;
    ExecutedTask *executed_tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    if (executed_tasks == NULL)
        return NULL;

    int executed_size = 0;

    process *executed = (process *)malloc(sizeof(process) * process_array_size);
    if (executed == NULL)
        return NULL;
    process execute;
    int execution_limit = -1;
    sort_process_array_by_at(process_array, process_array_size);
    while (!is_execution_done(executed, executed_size, process_array, process_array_size))
    {

        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at == current_time)
            {
                add_to_pr_queue_p(queue, process_array[i]);
                print_queue(queue);
            }
        }

        if (!is_pr_queue_empty(queue))
        {

            if (execution_limit == -1)
                execute = remove_from_pr_queue_p(queue);
            
            printf("%d", get_next_proc(queue, execute).priority != -1 && get_next_proc(queue, execute).priority == execute.priority);
            
            if (get_next_proc(queue, execute).priority != -1 && get_next_proc(queue, execute).priority == execute.priority)
            {
                execution_limit = get_at_process_with_higher_pr(queue, execute);
                if (execution_limit != -1)
                {
                    execute.execution_time -= execution_limit - current_time;
                    if (execute.execution_time <= 0)
                    {
                        add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, execution_limit, execute.name));
                        current_time = execution_limit;
                        if (!is_in_old_list(execute, executed, executed_size))
                        {
                            executed[executed_size] = execute;
                            executed_size++;
                        }
                    }
                    else
                    {
                        add_to_pr_queue_p(queue, execute);
                        current_time += execute.execution_time;
                    }
                }
                execution_limit = -1;
            }
            else
            {
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