ExecutedTask *get_multilevel_output(int quantum, process *process_array, int process_array_size, int *executed_tasks_size)
{

    *executed_tasks_size = 0;
    priority_queue *queue = init_priority_queue(100);
    if (queue == NULL)
    {
        printf("Allocation error");
    }
    int current_time = 0;
    ExecutedTask *executed_tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    int executed_size = 0;

    int monitor_quantum = 0;
    process last_exec_proc = (process){-1, -1, -1, ""};
    sort_process_array_by_at(process_array, process_array_size);
    while (executed_size < process_array_size)
    {

        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at == current_time && !is_in_queue(queue, process_array[i].name))
            {
                add_to_pr_queue_p(queue, process_array[i]);
                print_queue(queue, current_time);
            }
        }

        if (!is_pr_queue_empty(queue))
        {
            process execute = remove_from_pr_queue_p(queue);

            if (get_front(queue).priority != -1 && execute.priority == get_front(queue).priority)
            {
                while (get_front(queue).priority != -1 && execute.priority == get_front(queue).priority)
                {
                    if (strcmp(last_exec_proc.name, execute.name) == 0)
                        monitor_quantum = 1;
                    execute.execution_time -= 1;
                    last_exec_proc = execute;
                    printf("process %s exec 1 unit\n", execute.name);

                    monitor_quantum++;
                    printf("%d\n", monitor_quantum);

                    if (execute.execution_time > 0)
                    {
                        add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, current_time + 1, execute.arrived_at, execute.name));
                        current_time++;

                        for (int i = 0; i < process_array_size; i++)
                        {
                            if ((process_array[i].arrived_at == current_time) && !is_in_queue(queue, process_array[i].name))
                            {
                                add_to_pr_queue_p(queue, process_array[i]);
                            }
                        }

                        if (monitor_quantum == 2)
                        {
                            add_to_pr_queue_p(queue, execute);
                            monitor_quantum = 0;
                            break;
                        }
                    }
                    else
                    {

                        executed_size++;

                        add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, current_time + 1, execute.arrived_at, execute.name));

                        monitor_quantum = 0;
                        current_time++;
                        break;
                    }
                }
            }
            else
            {
                execute.execution_time--;
                last_exec_proc = execute;
                if (execute.execution_time > 0)
                {
                    add_to_pr_queue_p(queue, execute);
                }
                else
                {
                    executed_size++;
                }

                add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, current_time + 1, execute.arrived_at, execute.name));
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
