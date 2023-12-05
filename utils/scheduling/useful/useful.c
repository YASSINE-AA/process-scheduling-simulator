#include "useful.h"

bool is_in_old_list(process p, process *old_process_list, int old_process_list_size)
{
    if (old_process_list != NULL)
    {
        for (int j = 0; j < old_process_list_size; j++)
        {
            if (strncmp(p.name, old_process_list[j].name, sizeof(p.name)) == 0)
            {
                return true;
            }
        }
    }

    return false;
}


int get_earliest_time(process *process_array, int process_array_size)
{
    if (process_array_size == 0)
        return process_array[0].arrived_at;
    int min_value = get_earliest_time(process_array, process_array_size - 1);
    if (min_value > process_array[process_array_size - 1].arrived_at)
    {
        return process_array[process_array_size - 1].arrived_at;
    }
    else
    {
        return min_value;
    }
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
void free_process_array(process *arr)
{
    free(arr);
}
