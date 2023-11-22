
ExecutedTask get_task(int start, int finish, char *label)
{
    ExecutedTask task;
    task.label = strdup(label);
    task.start = start;
    task.finish = finish;
    return task;
}

void add_to_executed_tasks(ExecutedTask *executed_tasks, int *executed_tasks_size, ExecutedTask task)
{
    executed_tasks[*executed_tasks_size] = task;
    (*executed_tasks_size)++;
}
ExecutedTask *format_executed_tasks(ExecutedTask *tasks, int *tasks_size, process *process_array, int process_array_size)
{
    ExecutedTask current_task = tasks[0];
    ExecutedTask *new_tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    int new_tasks_size = 0;
    if (new_tasks == NULL)
    {
        printf("alloc failed");
    }

    for (int i = 1; i < *tasks_size; ++i)
    {

        if (strcmp(current_task.label, tasks[i].label) == 0 &&
            current_task.finish == tasks[i].start)
        {
            current_task.finish = tasks[i].finish; // Update the finish time
        }
        else
        {
            printf("Label: %s, Start: %d, Finish: %d\n",
                   current_task.label, current_task.start, current_task.finish);
            new_tasks[new_tasks_size] = current_task;
            (new_tasks_size)++;
            current_task = tasks[i];
        }
    }

    printf("Label: %s, Start: %d, Finish: %d\n",
           current_task.label, current_task.start, current_task.finish);
    new_tasks[new_tasks_size] = current_task;
    (new_tasks_size)++;
    new_tasks = (ExecutedTask *)realloc(new_tasks, sizeof(ExecutedTask) * new_tasks_size);

    if (new_tasks == NULL)
    {
        printf("realloc failed");
        free(tasks);
        return NULL;
    }
    (*tasks_size) = new_tasks_size;
    free(tasks);
    return new_tasks;
}

ExecutedTask *get_srt_output(process *process_array, int process_array_size, int *executed_tasks_size)
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
    while (!is_execution_done(executed, executed_size, process_array, process_array_size))
    {

        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at == current_time)
            {
                add_to_pr_queue(queue, process_array[i]);
            }
        }
        // execute
        if (!is_pr_queue_empty(queue))
        {
            process execute = remove_from_pr_queue(queue);

            if (!is_in_old_list(execute, executed, executed_size))
            {
                executed[executed_size] = execute;
                executed_size++;
            }
            execute.execution_time--;

            if (executed_size == process_array_size)
            {

                printf("what %s\n", execute.name);
                add_to_executed_tasks(executed_tasks, executed_tasks_size, get_task(current_time, current_time + execute.execution_time, execute.name));
                break;
            }
            if (execute.execution_time > 0)
            {
                add_to_pr_queue(queue, execute);
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
    else
        return NULL;
}
