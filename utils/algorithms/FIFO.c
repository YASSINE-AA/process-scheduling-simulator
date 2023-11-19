

ExecutedTask* get_fifo_output(process* process_array, int process_array_size, int* executed_tasks_size) {
    *executed_tasks_size = 0;
    proc_queue* queue = (proc_queue*) malloc(sizeof(proc_queue));
    if(queue == NULL) {
        printf("Allocation error");
    }
    create_queue(queue);
    int current_time = get_earliest_time(process_array, process_array_size);
    int last_timestamp = get_last_timestamp(process_array, process_array_size);
    int new_arrival_size = 0; 
    int in_queue_size = 0;
    int num_executed_processes = 0;
    process executed_processes[process_array_size];
    process in_queue[process_array_size];
    process* new_arrival = get_new_arrival(current_time, NULL, 0, process_array, process_array_size, &new_arrival_size);
    process* next_proc_in_q = next_available(new_arrival, new_arrival_size, NULL, 0);
    ExecutedTask* executed_tasks = (ExecutedTask*) malloc(sizeof(ExecutedTask)*100);

     while(!is_execution_done(executed_processes, num_executed_processes, process_array, process_array_size)) {
        if(next_proc_in_q != NULL && new_arrival != NULL) {
            //Populate queue
            while(next_proc_in_q != NULL) {
                add_to_queue(queue, *next_proc_in_q);    
                in_queue[in_queue_size] = *next_proc_in_q;
                in_queue_size ++;
     
                next_proc_in_q = next_available(new_arrival, new_arrival_size, in_queue, in_queue_size);   
            }

            //Execute
            while(!is_queue_empty(queue)) {
                process executed = remove_from_queue(queue);
                executed_processes[num_executed_processes] = executed;
                num_executed_processes++;  
                ExecutedTask executed_task;
                executed_task.start = current_time;
                executed_task.finish = current_time + executed.execution_time;
                executed_task.label = (const char*) strdup(executed.name);
             
                executed_tasks[*executed_tasks_size] = executed_task;
                (*executed_tasks_size)++;

                current_time += executed.execution_time;
                
             
            }
        }   else {
            current_time++;
           
        }     

        new_arrival = get_new_arrival(current_time, executed_processes, num_executed_processes, process_array, process_array_size, &new_arrival_size);
        next_proc_in_q = next_available(new_arrival, new_arrival_size, in_queue, in_queue_size);
    }
 
   
    if(new_arrival != NULL) free(new_arrival);
    if(queue != NULL) free(queue);
    return executed_tasks;
}