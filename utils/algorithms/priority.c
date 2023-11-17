




void get_priority_output(process *process_array, int process_array_size, bool is_preemptive)
{
    printf("============================== PRIORITY ========================================\n");
    proc_queue *queue = (proc_queue *)malloc(sizeof(proc_queue));
    if (queue == NULL)
    {
        printf("Allocation error");
    }
    create_queue(queue);
    int current_time = get_earliest_time(process_array, process_array_size);
    int last_timestamp = get_last_timestamp(process_array, process_array_size);
    int new_arrival_size = 0;
    int in_queue_size = 0;
    int num_executed_processes = 0;
    process *in_queue = (process *)malloc(sizeof(process) * process_array_size);

    process *executed_processes = (process *)malloc(sizeof(process) * process_array_size);
    if (executed_processes == NULL)
    {
        // Handle memory allocation failure
        return;
    }

    process *new_arrival = get_new_arrival(current_time, NULL, 0, process_array, process_array_size, &new_arrival_size);
    process *next_proc_in_q = next_available_p(new_arrival, new_arrival_size, NULL, 0);

while (!is_execution_done(executed_processes, num_executed_processes, process_array, process_array_size))
{
    if (next_proc_in_q != NULL && new_arrival != NULL)
    {
        // Populate queue
        while (next_proc_in_q != NULL)
        {
            add_to_queue(queue, *next_proc_in_q);
            in_queue[in_queue_size] = *next_proc_in_q;
            in_queue_size++;
            next_proc_in_q = next_available_p(new_arrival, new_arrival_size, in_queue, in_queue_size);
        }

        // Execute
        if (!is_preemptive)
        {
            while (!is_queue_empty(queue))
            {
                process executed = remove_from_queue(queue);
                remove_element(in_queue, &in_queue_size, executed);
                executed_processes[num_executed_processes] = executed;
                num_executed_processes++;
                printf("%s: [%d -> %d]  | ", executed.name, current_time, current_time + executed.execution_time);
                current_time += executed.execution_time;
            }
        }
        else
        {
            while(!is_queue_empty(queue)) {
                process to_be_executed = remove_from_queue(queue);
                remove_element(in_queue, &in_queue_size, to_be_executed);
                new_arrival = get_new_arrival(current_time + to_be_executed.execution_time, executed_processes, num_executed_processes, process_array, process_array_size, &new_arrival_size);
                next_proc_in_q = next_available_p(new_arrival, new_arrival_size, in_queue, in_queue_size);
                while(next_proc_in_q != NULL) {
               
                  if(next_proc_in_q->priority < to_be_executed.priority) {
                    if(next_proc_in_q->arrived_at - current_time < to_be_executed.execution_time) {   
                        to_be_executed.execution_time -= next_proc_in_q->arrived_at - current_time;
                        add_to_queue(queue, *next_proc_in_q);
                        in_queue[in_queue_size] = *next_proc_in_q;
                        in_queue_size++;
                        add_to_queue(queue, to_be_executed);
                        in_queue[in_queue_size] = to_be_executed;
                        in_queue_size++;
                        to_be_executed.execution_time = next_proc_in_q->arrived_at - current_time;
                        sort_queue_priority(queue); // sort the queue
                    } else {
                        add_to_queue(queue, *next_proc_in_q);
                        in_queue[in_queue_size] = *next_proc_in_q;
                        in_queue_size++;
                        sort_queue_priority(queue); // sort the queue
                    }
                    
                  }  else {
                    add_to_queue(queue, *next_proc_in_q);
                    in_queue[in_queue_size] = *next_proc_in_q;
                    in_queue_size++;
                    sort_queue_priority(queue);


                  }
                  next_proc_in_q = next_available_p(new_arrival, new_arrival_size, in_queue, in_queue_size);
               
                }
            
                remove_element(in_queue, &in_queue_size, to_be_executed);
                executed_processes[num_executed_processes] = to_be_executed;
                num_executed_processes++;
                printf("%s: [%d -> %d]  | ",  to_be_executed.name, current_time, current_time +  to_be_executed.execution_time);
                current_time +=  to_be_executed.execution_time;
                
            }
        }
    }
    else
    {
        current_time++;
     
    }
    
    // Update new_arrival and next_proc_in_q for the next iteration
    new_arrival = get_new_arrival(current_time, executed_processes, num_executed_processes, process_array, process_array_size, &new_arrival_size);
    next_proc_in_q = next_available_p(new_arrival, new_arrival_size, in_queue, in_queue_size);
}


    if (executed_processes != NULL)
    {
        if (num_executed_processes < process_array_size)
        {
            process *resized_executed = realloc(executed_processes, sizeof(process) * num_executed_processes);
            if (resized_executed != NULL)
            {
                free(executed_processes);
            }
            else
            {
                printf("Reallocation failed! \n");
            }
        }
    }
  
    if (new_arrival != NULL)
        free(new_arrival);
    if (queue != NULL)
        free(queue);
    printf("\n============================== PRIORITY ========================================\n");
}