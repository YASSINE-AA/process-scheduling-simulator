bool is_queue_empty(proc_queue* q) {
    return q->head == NULL;
}

void create_queue(proc_queue* q) {
    q->head = NULL;
    q->tail = NULL;
}
void add_to_queue(proc_queue* q, process proc) {
    proc_in_queue* new_process = (proc_in_queue*)malloc(sizeof(proc_in_queue));
    if (new_process == NULL) {
        // Handle memory allocation failure
        return;
    }
    new_process->value = proc;
    new_process->next = NULL;

    if (is_queue_empty(q)) {
        q->head = new_process;
        q->tail = new_process;
    } else {
        q->tail->next = new_process;
        q->tail = new_process;
    }
}

process remove_from_queue(proc_queue* q) {
    process empty_process; // A process with some default values
    empty_process.name[0] = '\0';
    empty_process.arrived_at = 0;
    empty_process.execution_time = -1;
    
    if (!is_queue_empty(q)) {
        proc_in_queue* proc_to_delete = q->head;
        q->head = q->head->next;
        process result = proc_to_delete->value;
        free(proc_to_delete);
        return result;
    }
    
    return empty_process; // Return a default process if the queue is empty
}

void sort_queue_priority(proc_queue* q) {
    for (proc_in_queue* i = q->head; i != NULL; i = i->next) {
        for (proc_in_queue* j = i->next; j != NULL; j = j->next) {
            if (i->value.priority > j->value.priority) {
                process temp = i->value;
                i->value = j->value;
                j->value = temp;
            }
        }
    }
}
void sort_queue_srt(proc_queue* q) {
    for (proc_in_queue* i = q->head; i != NULL; i = i->next) {
        for (proc_in_queue* j = i->next; j != NULL; j = j->next) {
            if (i->value.execution_time > j->value.execution_time) {
                process temp = i->value;
                i->value = j->value;
                j->value = temp;
            }
        }
    }
}


