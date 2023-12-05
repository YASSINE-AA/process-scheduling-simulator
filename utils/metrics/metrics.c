#include "metrics.h"


int get_rotation_time(const char *process_name, ExecutedTask *executed, int executed_size)
{
    int result = 0;
    int finish = 0;
    int arrival_time = 0;
    for (int i = 0; i < executed_size; i++)
    {
        if (strcmp(process_name, executed[i].label) == 0)
        {
            finish = executed[i].finish;
            arrival_time = executed[i].arrival_time;
        }
    }
    return finish - arrival_time;
}

int get_waiting_time(const char *process_name, ExecutedTask *executed, int executed_size, process *process_array, int process_array_size)
{
    int rotation_time = get_rotation_time(process_name, executed, executed_size);
    int burst_time = 0;
    for (int i = 0; i < process_array_size; i++)
    {
        if (strcmp(process_name, process_array[i].name) == 0)
        {
            burst_time = process_array[i].execution_time;
            break;
        }
    }
    return rotation_time - burst_time;
}
