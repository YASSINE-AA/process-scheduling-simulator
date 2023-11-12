void get_round_robin_output(int quantum, process* process_array, int number_of_process) {
    int counter = get_earliest_time(process_array, number_of_process);

    proc_queue* queue = (proc_queue*)malloc(sizeof(proc_queue));
    if (queue == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Initialize the queue
    create_queue(queue);
    process* executed_processes = (process*) malloc(sizeof(process) * number_of_process);
    int num_executed_processes = 0;
    int new_arrivals_size = 0;
    int last_execution = 0;
    process* new_arrivals = get_new_arrival(counter, NULL, new_arrivals_size, process_array, number_of_process, &new_arrivals_size);
    process* next_available_proc = next_available(new_arrivals, new_arrivals_size, next_available_proc);
    while (counter <= get_last_timestamp(process_array, number_of_process)) {
     
        if(new_arrivals !=NULL) {
            while (next_available_proc != NULL) {
                    add_to_queue(queue, *next_available_proc);
                    next_available_proc = next_available(new_arrivals, new_arrivals_size, next_available_proc);
            }
        
            while (!is_queue_empty(queue)) {
                    process executed = remove_from_queue(queue);

                    int old_counter = counter;
                    executed_processes[num_executed_processes] = executed;
                    num_executed_processes++;
                    if (executed.execution_time > 0) {
                        if (executed.execution_time <= quantum) {
                            counter += executed.execution_time;
                            executed.execution_time = 0;
                            
                        } else {
                            counter += quantum;
                            executed.execution_time -= quantum;
                            add_to_queue(queue, executed);
                            
               
                        } 
                        printf("%s [%d -> %d] ! ", executed.name, old_counter-1, counter-1);
                        
                    }
            }
        }
         
    
        new_arrivals = get_new_arrival(counter, executed_processes, num_executed_processes, process_array, number_of_process, &new_arrivals_size);
        next_available_proc = next_available(new_arrivals, new_arrivals_size, next_available_proc);
    
    counter++;
    
    }
    free(new_arrivals);
    free(queue); // Free the queue
}
