// Function to check if a process is in an old process list
bool is_in_old_list(process p, process* old_process_list, int old_process_list_size) {
    if (old_process_list != NULL) {
        for (int j = 0; j < old_process_list_size; j++) {
            if (strncmp(p.name, old_process_list[j].name, sizeof(p.name)) == 0) {
                return true;
            }
        }
    }

    return false;
}

// Function to get available processes at a given time
process* who_available(int current_time, process* process_array, int process_array_size, int* number_available_processes) {
    if (process_array == NULL || process_array_size <= 0) {
        return NULL;
    }

    process* available = (process*)malloc(process_array_size * sizeof(process));
    if (available == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    *number_available_processes = 0;

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

    return available;
}

// Function to get new arrivals at a given timestamp
process* get_new_arrival(int timestamp, process* old_process_list, int old_process_list_size, process* process_list, int process_list_size, int* new_arrivals_size) {
    *new_arrivals_size = 0; // Initialize to 0
    process* new_arrivals = (process*)malloc(process_list_size * sizeof(process));
    if (new_arrivals == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    int num_available_process = 0;
    process* available_at_t = who_available(timestamp, process_list, process_list_size, &num_available_process);

    if (available_at_t != NULL) {
        for (int i = 0; i < num_available_process; i++) {
            if (!is_in_old_list(available_at_t[i], old_process_list, old_process_list_size)) {
                new_arrivals[*new_arrivals_size] = available_at_t[i];
                (*new_arrivals_size)++;
            }
        }
    }

    free(available_at_t);

    if (*new_arrivals_size == 0) {
        free(new_arrivals);
        return NULL;
    }

    return new_arrivals;
}

// Function to find the next available process based on arrival time
process* next_available(process* available_processes, int number_available_processes, process* in_queue, int proc_in_queue) {
    if (available_processes != NULL) {
        process* earliest = NULL;

        for (int i = 0; i < number_available_processes; i++) {
            if (!is_in_old_list(available_processes[i], in_queue, proc_in_queue)) {
                if (earliest == NULL || available_processes[i].arrived_at <= earliest->arrived_at) {
                    earliest = &available_processes[i];
                }
            }
        }

        return earliest;
    }

    return NULL;
}


void remove_element(process *array, int *size, process elementToRemove) {
    int found = 0;
    for (int i = 0; i < *size; ++i) {
        if (array[i].name == elementToRemove.name) {
            found = 1;
            for (int j = i; j < *size - 1; ++j) {
                array[j] = array[j + 1];
            }
            (*size)--;
            break;
        }
    }

    if (!found) {
        printf("Element not found in the array.\n");
    }
}


// Function to get the earliest arrival time in a process array
int get_earliest_time(process* process_array, int process_array_size) {
    if (process_array_size == 0) return process_array[0].arrived_at;
    int min_value = get_earliest_time(process_array, process_array_size - 1);
    if (min_value > process_array[process_array_size - 1].arrived_at) {
        return process_array[process_array_size - 1].arrived_at;
    } else {
        return min_value;
    }
}

// Function to get the latest arrival time in a process array
int get_latest_time(process* process_array, int process_array_size) {
    int latest = process_array[0].arrived_at;
    for (int i = 0; i < process_array_size; i++) {
        if (latest < process_array[i].arrived_at) {
            latest = process_array[i].arrived_at;
        }
    }
    return latest;
}

// Function to get the total execution time of processes in an array
int get_total_execution_time(process* process_array, int process_array_size) {
    int sum = 0;

    for (int i = 0; i < process_array_size; i++) {
        sum += process_array[i].execution_time;
    }

    return sum;
}

// Function to get the last timestamp considering the execution times
int get_last_timestamp(process* process_array, int process_array_size) {
    int latest_time = get_latest_time(process_array, process_array_size);
    int earliest_time = get_earliest_time(process_array, process_array_size);
    int total_exec_time = get_total_execution_time(process_array, process_array_size) - process_array[process_array_size - 1].execution_time + earliest_time;

    if (total_exec_time >= latest_time) {
        return total_exec_time + earliest_time + process_array[process_array_size - 1].execution_time;
    } else {
        return latest_time + process_array[process_array_size - 1].execution_time;
    }
}

// Function to check if execution of all processes is done
bool is_execution_done(process* executed, int executed_size, process* process_array, int process_array_size) {
    bool equal = true;

    if (executed != NULL && process_array != NULL) {
        for (int i = 0; i < process_array_size; i++) {
            bool process_executed = false;

            for (int j = 0; j < executed_size; j++) {
                if (strncmp(process_array[i].name, executed[j].name, sizeof(process_array[i].name)) == 0) {
                    process_executed = true;
                    break;
                }
            }

            if (!process_executed) {
                equal = false;
                break;
            }
        }
    } else {
        equal = false;
    }

    return equal;
}

// Free memory allocated for process arrays
void free_process_array(process* arr) {
    free(arr);
}
