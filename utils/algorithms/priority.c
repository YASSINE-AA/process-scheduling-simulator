
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
    bool lock = false;
    bool added = false;
    int executed_size = 0;
    bool removed = false;

    process execute = (process){-1, -1, -1, ""};
    process *executed = (process *)malloc(sizeof(process) * process_array_size);
    if (executed == NULL)
        return NULL;
    sort_process_array_by_at(process_array, process_array_size);
    while (!is_execution_done(executed, executed_size, process_array, process_array_size))
    {

        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at == current_time)
            {
                add_to_pr_queue_p(queue, process_array[i]);
                print_queue(queue, current_time);
            }
        }

        if (!is_pr_queue_empty(queue))
        {

            if (get_front(queue).priority != -1 && get_front(queue).priority < execute.priority && !is_pr_queue_empty(queue))
                lock = false;

            if (!lock)
            {
                execute = remove_from_pr_queue_p(queue);
                removed = true;
            }

            execute.execution_time -= 1;
            if (get_front(queue).priority != -1 && get_front(queue).priority == execute.priority)
            {
                printf("%s %d", execute.name, execute.execution_time);
                lock = true;
                if (execute.execution_time > 0)
                {
                    if (removed)
                    {
                        add_to_pr_queue_p(queue, execute);
                        removed = false;
                    }
                }
                else
                {
                    if (!is_in_old_list(execute, executed, executed_size))
                    {
                        executed[executed_size] = execute;
                        executed_size++;
                    }
                    lock = false;
                }
            }

            if (execute.execution_time > 0 && !lock)
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
            lock = false;
            current_time++;
        }
    }
    executed_tasks = format_executed_tasks(executed_tasks, executed_tasks_size, process_array, process_array_size);
    free_priority_queue(queue);
    if (executed_tasks != NULL)
        return executed_tasks;
    return NULL;
}