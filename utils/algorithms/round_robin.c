ExecutedTask *get_round_robin_output(int quantum, process *process_array, int number_of_process, int *tasks_size)
{
    
    int counter = get_earliest_time(process_array, number_of_process);
    *tasks_size = 0;
    ExecutedTask *tasks = NULL;
    int tasks_capacity = 100;

    if ((tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * tasks_capacity)) == NULL)
    {
        // Handle memory allocation failure
        return NULL;
    }

    proc_queue *queue = (proc_queue *)malloc(sizeof(proc_queue));
    if (queue == NULL)
    {
        // Handle memory allocation failure
        free(tasks);
        return NULL;
    }

    // Initialize the queue
    create_queue(queue);

    process *executed_processes = (process *)malloc(sizeof(process) * 100);
    process in_queue[number_of_process];

    int num_executed_processes = 0;
    int new_arrivals_size = 0;
    int in_queue_size = 0;

    process *new_arrivals = get_new_arrival(counter, NULL, new_arrivals_size, process_array, number_of_process, &new_arrivals_size);
    process *next_available_proc = next_available(new_arrivals, new_arrivals_size, NULL, 0);
    while (!is_execution_done(executed_processes, num_executed_processes, process_array, number_of_process))
    {
        if (new_arrivals != NULL && next_available_proc != NULL)
        {
            // Add to queue
            while (next_available_proc != NULL)
            {
                add_to_queue(queue, *next_available_proc);
                in_queue[in_queue_size] = *next_available_proc;
                in_queue_size++;
                next_available_proc = next_available(new_arrivals, new_arrivals_size, in_queue, in_queue_size);
            }

            // Execute queue
            while (!is_queue_empty(queue))
            {
                process executed = remove_from_queue(queue);

                int old_counter = counter;
                executed_processes[num_executed_processes] = executed;
                num_executed_processes++;

                if (executed.execution_time > 0)
                {
                    if (executed.execution_time <= quantum)
                    {
                        counter += executed.execution_time;
                        executed.execution_time = 0;

                        ExecutedTask task;
                        task.arrival_time = executed.arrived_at;
                        task.start = old_counter;
                        task.finish = counter;
                        task.label = strdup(executed.name);

                        if (task.label == NULL)
                        {
                            free(tasks);
                            free(queue);
                            return NULL;
                        }

                        if (*tasks_size < tasks_capacity)
                        {
                            tasks[*tasks_size] = task;
                            (*tasks_size)++;
                        }
                        else
                        {
                            tasks_capacity *= 2;
                            tasks = realloc(tasks, sizeof(ExecutedTask) * tasks_capacity);

                            if (tasks == NULL)
                            {
                                free(queue);
                                return NULL;
                            }

                            tasks[*tasks_size] = task;
                            (*tasks_size)++;
                        }
                    }
                    else
                    {
                        counter += quantum;
                        executed.execution_time -= quantum;

                        ExecutedTask task;
                        task.arrival_time = executed.arrived_at;
                        
                        task.start = old_counter;
                        task.finish = counter;
                        task.label = strdup(executed.name);

                        if (task.label == NULL)
                        {
                            // Handle strdup failure
                            free(tasks);
                            free(queue);
                            return NULL;
                        }

                        if (*tasks_size < tasks_capacity)
                        {
                            tasks[*tasks_size] = task;
                            (*tasks_size)++;
                        }
                        else
                        {
                            tasks_capacity *= 2;
                            tasks = realloc(tasks, sizeof(ExecutedTask) * tasks_capacity);

                            if (tasks == NULL)
                            {
                                free(queue);
                                return NULL;
                            }

                            tasks[*tasks_size] = task;
                            (*tasks_size)++;
                        }

                        new_arrivals = get_new_arrival(counter, executed_processes, num_executed_processes, process_array, number_of_process, &new_arrivals_size);
                        next_available_proc = next_available(new_arrivals, new_arrivals_size, in_queue, in_queue_size);

                        if (next_available_proc != NULL)
                        {
                            while (next_available_proc != NULL)
                            {
                                add_to_queue(queue, *next_available_proc);
                                in_queue[in_queue_size] = *next_available_proc;
                                in_queue_size++;

                                next_available_proc = next_available(new_arrivals, new_arrivals_size, in_queue, in_queue_size);
                            }
                        }

                        add_to_queue(queue, executed);
                    }
                }
            }
        }
        else
        {
            counter++;
            printf("counter: %d\n", counter);
        }

        new_arrivals = get_new_arrival(counter, executed_processes, num_executed_processes, process_array, number_of_process, &new_arrivals_size);
        next_available_proc = next_available(new_arrivals, new_arrivals_size, in_queue, in_queue_size);
    }

    if (executed_processes != NULL)
    {
        if (num_executed_processes < number_of_process)
        {
            process *resized_ep = (process *)realloc(executed_processes, num_executed_processes);
            if (resized_ep != NULL)
            {
                free(resized_ep);
                resized_ep = NULL;
            }
            else
                return NULL;
        }
    }
    // Free allocated memory
    if (queue != NULL)
    {
        free(queue);
    }

    return tasks;
}