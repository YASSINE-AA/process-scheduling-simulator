
void get_fifo_output(process* process_array, int number_of_process) {
    int number_available_processes = 0; 
    int current_time = 0;
    proc_queue* queue = (proc_queue*)malloc(sizeof(proc_queue));

    if (queue == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Initialize the queue
    create_queue(queue);
    process* available_at_t = who_available(get_latest_time(process_array, number_of_process), process_array, number_of_process, &number_available_processes);

    for (int i = 0; i < number_available_processes; i++) {
        process* active_proc = next_available(available_at_t, number_available_processes, NULL, 0);
        add_to_queue(queue, *active_proc);
        active_proc->execution_time = -1;
    }
    
    free(available_at_t);
    
    // Display the queue
    while (!is_queue_empty(queue)) {
        process executed = remove_from_queue(queue);
        if(!(current_time > executed.arrived_at)) current_time = executed.arrived_at;        
        printf("%s: [%d -> %d]  | ", executed.name, current_time, current_time + executed.execution_time); 
        current_time += executed.execution_time;
    }
    printf("\n");
    free(queue); // Free the queue
}