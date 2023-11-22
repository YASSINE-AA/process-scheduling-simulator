#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int arrival_time;
    int start;
    int finish;
    const char* label;
    int color[4];
} ExecutedTask;

typedef struct
{
    int arrived_at;
    int execution_time;
    int priority;
    char name[100];
} process;

void format_executed_tasks(ExecutedTask* tasks, int tasks_size, process* process_array, int process_array_size) {
    // Assuming the tasks array is already sorted based on arrival_time and start
    // If not, you may want to sort it before proceeding

    // Initialize variables to track the current task
    ExecutedTask current_task = tasks[0];

    for (int i = 1; i < tasks_size; ++i) {
        if (strcmp(current_task.label, tasks[i].label) == 0 &&
            current_task.start + 1 == tasks[i].start) {
            // If the current task has the same label and consecutive start integers
            current_task.finish = tasks[i].finish; // Update the finish time
        } else {
            // Print or process the reduced task
            printf("Label: %s, Start: %d, Finish: %d\n",
                   current_task.label, current_task.start, current_task.finish);

            // Update the current task for the next iteration
            current_task = tasks[i];
        }
    }

    // Print or process the last reduced task
    printf("Label: %s, Start: %d, Finish: %d\n",
           current_task.label, current_task.start, current_task.finish);
}

int main() {
    // Example usage
    ExecutedTask tasks[] = {
        {0, 1, 2, "Task1", {255, 0, 0, 255}},
        {0, 2, 3, "Task1", {255, 0, 0, 255}},
                {0, 3, 4, "Task1", {255, 0, 0, 255}},

        {0, 5, 7, "Task2", {0, 255, 0, 255}},
        
        // Add more tasks as needed
    };

    process processes[] = {
        {0, 2, 1, "Process1"},
        {0, 3, 2, "Process2"},
        // Add more processes as needed
    };

    int tasks_size = sizeof(tasks) / sizeof(tasks[0]);
    int process_array_size = sizeof(processes) / sizeof(processes[0]);

    format_executed_tasks(tasks, tasks_size, processes, process_array_size);

    return 0;
}