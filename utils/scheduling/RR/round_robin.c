
#include "round_robin.h"

ExecutedTask *get_round_robin_output(int quantum, process *process_array, int process_array_size, int *tasks_size)
{
    *tasks_size = 0;
    int current_time = get_earliest_time(process_array, process_array_size);
    int executed_size = 0;
    int temp_q = 0;
    int in_queue_size = 0;
    int min_index = 0;

  
    process *in_queue = (process *)malloc(sizeof(process) * process_array_size);
    if (in_queue == NULL)
    {
        printf("Allocation failed!");
        return NULL;
    }

    proc_queue *queue = (proc_queue *)malloc(sizeof(proc_queue));
    if (queue == NULL)
    {
        printf("Allocation failed!");
        return NULL;
    }

    ExecutedTask *tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    if (tasks == NULL)
    {
        printf("Allocation failed!");
        return NULL;
    }
    sort_process_array_by_at(process_array, process_array_size);
    while (executed_size < process_array_size)
    {

        for (int i = 0; i < process_array_size; i++)
        {
            if (process_array[i].arrived_at <= current_time && !is_in_old_list(process_array[i], in_queue, in_queue_size))
            {
                in_queue[in_queue_size] = process_array[i];
                in_queue_size++;
                add_to_queue(queue, process_array[i]);
            }
        }
        if (!is_queue_empty(queue))
        {
            process execute = remove_from_queue(queue);

            if (execute.execution_time >= quantum)
            {
                execute.execution_time -= quantum;
                add_to_executed_tasks(tasks, tasks_size, get_task(current_time, current_time + quantum, execute.arrived_at, execute.name));

                if (execute.execution_time == 0)
                {
                    
                    executed_size++;
                    current_time += quantum;
                }
                else
                {
                    current_time += quantum;

                    for (int i = 0; i < process_array_size; i++)
                    {
                        if (process_array[i].arrived_at <= current_time && !is_in_old_list(process_array[i], in_queue, in_queue_size))
                        {
                            in_queue[in_queue_size] = process_array[i];
                            in_queue_size++;
                            add_to_queue(queue, process_array[i]);
                        }
                    }
                    add_to_queue(queue, execute);
                }
            }
            else
            {
                add_to_executed_tasks(tasks, tasks_size, get_task(current_time, current_time + execute.execution_time, execute.arrived_at, execute.name));
                executed_size++;
                current_time += execute.execution_time;
            }
        }
        else
        {
            current_time++;
        }
    }

    tasks = format_executed_tasks(tasks, tasks_size, process_array, process_array_size);

    if (tasks != NULL)
        return tasks;
    return NULL;
}