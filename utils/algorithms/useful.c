

process* who_available(int current_time, process* process_array, int process_array_size, int* number_available_processes) {
    *number_available_processes = 0;
    if (process_array == NULL || process_array_size <= 0) {
        return NULL;
    }

    process* available = malloc(process_array_size * sizeof(process));
    if (available == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < process_array_size; i++) {
        if (process_array[i].arrived_at <= current_time) {
            available[*number_available_processes] = process_array[i];
            (*number_available_processes)++;
        }
    }

    if (*number_available_processes == 0) {
        free(available); 
        return NULL;
    }

    available = realloc(available, *number_available_processes * sizeof(process));

    return available;
}

process* next_available(process* available_processes, int number_available_processes) {
    process* earliest = NULL; // Initialize earliest to NULL

    if (available_processes != NULL) {
        for (int i = 0; i < number_available_processes; i++) {
            if (available_processes[i].execution_time > 0) {
                if (earliest == NULL || available_processes[i].arrived_at <= earliest->arrived_at) {
                    earliest = &available_processes[i];
                }
            }
        }
    }

    return earliest;
}
 
int get_latest_time(process* process_array, int process_array_size) {
    int latest = process_array[0].arrived_at;
    for(int i=0; i<process_array_size; i++) {
        if(latest <process_array[i].arrived_at) {
            latest = process_array[i].arrived_at;
        }
    }
    return latest; 
}