ExecutedTask* get_srt_output(process* process_array, int process_array_size, int* tasks_size) {
    *tasks_size = 0;
    ExecutedTask* tasks = (ExecutedTask*) malloc(sizeof(ExecutedTask)*process_array_size);

    proc_queue* queue = (proc_queue*) malloc(sizeof(proc_queue));
    if(queue == NULL) {
        printf("Error allocation");
        return NULL;
    }
    process* executed_processes = (process*) malloc(sizeof(process) * process_array_size);
     if(executed_processes == NULL) {
        printf("Error allocation");
        return NULL;
    }
    process* in_queue = (process*) malloc(sizeof(process) * process_array_size);
    if(in_queue == NULL) {
        printf("Error allocation");
        return NULL;
    }

    

    int executed_processes_size = 0;
    int in_queue_size = 0;
    int new_arrivals_size = 0;
    int current_time = get_earliest_time(process_array, process_array_size);
    create_queue(queue);

    process* new_arrivals = get_new_arrival(current_time, NULL, 0, process_array, process_array_size, &new_arrivals_size);
    process* next_proc_in_q = next_available_srt(new_arrivals, new_arrivals_size, NULL, 0);

    while(!is_execution_done(executed_processes, executed_processes_size, process_array, process_array_size)) {
        if(new_arrivals != NULL && next_proc_in_q != NULL) {
            
            //Populate
            while(next_proc_in_q != NULL) {
                add_to_queue(queue, *next_proc_in_q);
                in_queue[in_queue_size] = *next_proc_in_q;
                in_queue_size++;
                next_proc_in_q = next_available_srt(new_arrivals, new_arrivals_size, in_queue, in_queue_size);
            }

            //Execution
            while(!is_queue_empty(queue)) {
                process to_be_executed = remove_from_queue(queue);
                
                new_arrivals = get_new_arrival(current_time + to_be_executed.execution_time, executed_processes, executed_processes_size,  process_array, process_array_size,&new_arrivals_size);
                next_proc_in_q = next_available_srt(new_arrivals, new_arrivals_size, in_queue, in_queue_size);
                while(next_proc_in_q != NULL) {
                    if(to_be_executed.execution_time > next_proc_in_q->execution_time) {
                        if(next_proc_in_q->arrived_at - current_time < to_be_executed.execution_time) {   
                            to_be_executed.execution_time -= next_proc_in_q->arrived_at - current_time;
                            add_to_queue(queue, *next_proc_in_q);
                            in_queue[in_queue_size] = *next_proc_in_q;
                            in_queue_size ++;
                            add_to_queue(queue, to_be_executed);
                            in_queue[in_queue_size] = *next_proc_in_q;
                            in_queue_size ++;
                            sort_queue_srt(queue);
                            to_be_executed.execution_time = next_proc_in_q->arrived_at - current_time;  
                        } else {
                             add_to_queue(queue, *next_proc_in_q);
                            in_queue[in_queue_size] = *next_proc_in_q;
                            in_queue_size ++;
                            sort_queue_srt(queue);
                        }
                    } else {
                        add_to_queue(queue, *next_proc_in_q);
                        in_queue[in_queue_size] = *next_proc_in_q;
                        in_queue_size ++;
                        sort_queue_srt(queue);
                    }
                    next_proc_in_q = next_available_srt(new_arrivals, new_arrivals_size, in_queue, in_queue_size); 
                }
            remove_element(in_queue, &in_queue_size, to_be_executed);
            executed_processes[executed_processes_size] = to_be_executed;
            executed_processes_size++;
            ExecutedTask task;
            task.start = current_time;
            task.finish = current_time + to_be_executed.execution_time;
            task.label = (const char*) strdup(to_be_executed.name);
            tasks[*tasks_size] = task;
            (*tasks_size)++;
            printf("%d \n", *tasks_size);
          
            current_time +=  to_be_executed.execution_time;

            }


        } else {
            current_time++;
        }

        new_arrivals = get_new_arrival(current_time, executed_processes, executed_processes_size, process_array, process_array_size, &new_arrivals_size);
        next_proc_in_q = next_available_srt(new_arrivals, new_arrivals_size, in_queue, in_queue_size);
        


    } 

    
   
  
  
    if (executed_processes_size < process_array_size)
{
    process *resized_executed = realloc(executed_processes, sizeof(process) * executed_processes_size);
    if (resized_executed != NULL)
    {
        executed_processes = resized_executed;  // Update the pointer
    }
    else
    {
        fprintf(stderr, "Reallocation failed! \n");
   #
    }
}
    if (new_arrivals != NULL)
        free(new_arrivals);
    if (queue != NULL)
        free(queue);
    
    return tasks;
}