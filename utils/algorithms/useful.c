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


int get_earliest_time(process* process_array, int process_array_size) {
    if (process_array_size == 0) return process_array[0].arrived_at;
    int min_value = get_earliest_time(process_array, process_array_size - 1);
    if (min_value > process_array[process_array_size - 1].arrived_at) {
        return process_array[process_array_size - 1].arrived_at;
    } else {
        return min_value;
    }
}

int get_latest_time(process* process_array, int process_array_size) {
    int latest = process_array[0].arrived_at;
    for (int i = 0; i < process_array_size; i++) {
        if (latest < process_array[i].arrived_at) {
            latest = process_array[i].arrived_at;
        }
    }
    return latest;
}

int get_total_execution_time(process* process_array, int process_array_size) {
    int sum = 0;

    for (int i = 0; i < process_array_size; i++) {
        sum += process_array[i].execution_time;
    }

    return sum;
}

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

void sort_process_array_by_at(process *process_array, int process_array_size)
{
    for (int i = 0; i < process_array_size - 1; i++)
    {
        for (int j = 0; j < process_array_size - i - 1; j++)
        {
            if (process_array[j].arrived_at > process_array[j + 1].arrived_at)
            {
                
                process temp = process_array[j];
                process_array[j] = process_array[j + 1];
                process_array[j + 1] = temp;
            }
        }
    }
}
void free_process_array(process* arr) {
    free(arr);
}
