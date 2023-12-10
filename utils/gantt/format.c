#include "format.h"

ExecutedTask get_task(int start, int finish, int arrival_time, char *label)
{
    ExecutedTask task;
    task.label = strdup(label);
    task.start = start;
    task.finish = finish;
    task.arrival_time = arrival_time;

    
    task.color[0] = (double)rand() / RAND_MAX;
    task.color[1] = (double)rand() / RAND_MAX;
    task.color[2] = (double)rand() / RAND_MAX;
    task.color[3] = (double)rand() / RAND_MAX;

    return task;
}

void add_to_executed_tasks(ExecutedTask *executed_tasks, int *executed_tasks_size, ExecutedTask task)
{
    executed_tasks[*executed_tasks_size] = task;
    (*executed_tasks_size)++;
}

ExecutedTask *format_executed_tasks(ExecutedTask *tasks, int *tasks_size, process *process_array, int process_array_size)
{
    ExecutedTask current_task = tasks[0];
    ExecutedTask *new_tasks = (ExecutedTask *)malloc(sizeof(ExecutedTask) * 100);
    int new_tasks_size = 0;
    if (new_tasks == NULL)
    {
        printf("alloc failed");
    }

    for (int i = 1; i < *tasks_size; ++i)
    {

        if (strcmp(current_task.label, tasks[i].label) == 0 &&
            current_task.finish == tasks[i].start)
        {
            current_task.finish = tasks[i].finish; 
        }
        else
        {
                   new_tasks[new_tasks_size] = current_task;
                   (new_tasks_size)++;
                   current_task = tasks[i];
        }
    }

           new_tasks[new_tasks_size] = current_task;
           (new_tasks_size)++;
           new_tasks = (ExecutedTask *)realloc(new_tasks, sizeof(ExecutedTask) * new_tasks_size);

           if (new_tasks == NULL)
           {
               printf("realloc failed");
               free(tasks);
               return NULL;
           }
           (*tasks_size) = new_tasks_size;
           free(tasks);
           return new_tasks;
}
