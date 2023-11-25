
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
    int arrival_offset_size = 0;
    process *arrival_offset = (process *)malloc(sizeof(process) * process_array_size);
    if (arrival_offset == NULL)
        return NULL;

    process *executed = (process *)malloc(sizeof(process) * process_array_size);
    if (executed == NULL)
        return NULL;
    bool finish_cond = false;
    process on_hold = (process) {0, 0, -1, ""};
    while (!is_execution_done(executed, executed_size, process_array, process_array_size))
    {

        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at == current_time && !is_in_old_list(process_array[i], arrival_offset, arrival_offset_size))
            {
                add_to_pr_queue_p(queue, process_array[i], finish_track_arr, finish_track_arr_size, current_time);
            }
        }

        if (!is_pr_queue_empty(queue))
        {
            process execute = remove_from_pr_queue_p(queue);
        
            execute.execution_time--;

            if (execute.execution_time > 0)
            {
                add_to_pr_queue_p(queue, execute, finish_track_arr, finish_track_arr_size, current_time);
            }
            else
            {
                if (!is_in_old_list(execute, executed, executed_size))
                {
                    executed[executed_size] = execute;
                    executed_size++;
                }
            }

            // add to finish tracker
            finish_track_arr = update_finish_tracker(finish_track_arr, &finish_track_arr_size, execute, 1, current_time);

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