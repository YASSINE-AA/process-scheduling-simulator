
#include "write.h"

bool modify_quantum_val(int new_value)
{
    char *filename = "generated_config.json";
    FILE *fp = fopen(filename, "r+");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *config = (char *)malloc(file_size + 1);
    if (config == NULL)
    {
        printf("Error: memory allocation failed\n");
        fclose(fp);
        return false;
    }

    fread(config, 1, file_size, fp);
    config[file_size] = '\0';

    fclose(fp);

    cJSON *config_json = cJSON_Parse(config);
    if (config_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(config);
        return NULL;
    }

    const cJSON *nested_options = NULL;
    const cJSON *options_list = NULL;
    const cJSON *quantum_option = NULL;

    options_list = cJSON_GetObjectItemCaseSensitive(config_json, "options");
    quantum_option = cJSON_GetObjectItemCaseSensitive(options_list, "quantum");

    cJSON_SetNumberValue(quantum_option, new_value);

    char *string = cJSON_Print(config_json);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print config_file.\n");
    }

    cJSON_Delete(config_json);
    const char *config_file_name = "generated_config.json";
    write_to_config(string);

    return true;
}

void set_ranges(char range[100], int *range_start, int *range_end)
{
    *range_start = -1;
    *range_end = -1;
    char *token = strtok(range, "-");

    while (token != NULL)
    {
        if (*range_start == -1)
        {
            *range_start = atoi(token);
        }
        else if (*range_end == -1)
        {
            *range_end = atoi(token);
        }

        token = strtok(NULL, "-");
    }
}

int create_random_process_array(process processes[100], int max_proc_range_start, int max_proc_range_end, int exec_range_start, int exec_range_end, int priority_range_start, int priority_range_end, int arrival_range_start, int arrival_range_end)
{

    srand(time(NULL));
    int dim = rand() % (max_proc_range_end - max_proc_range_start) + max_proc_range_start;
    for (int i = 0; i < dim; i++)
    {
        processes[i].arrived_at = rand() % (arrival_range_end - arrival_range_start) + arrival_range_start;
        processes[i].execution_time = rand() % (exec_range_end - exec_range_start) + exec_range_start;
        processes[i].priority = rand() % (priority_range_end - priority_range_start) + priority_range_start;
        snprintf(processes[i].name, sizeof(processes[i].name), "p%d", i + 1);
    }

    return dim;
}

void write_to_config(const char *content)
{
    FILE *fptr = fopen("generated_config.json", "w");
    fprintf(fptr, "%s", content);
    fclose(fptr);
}

void *generate_config_file(options ops, char *max_proc_range, char *exec_range, char *priority_range, char *arrival_range)
{
    srand(time(NULL));

    process resolution_numbers[100];

    int max_proc_range_start;
    int max_proc_range_end;

    int exec_range_start;
    int exec_range_end;

    int priority_range_start;
    int priority_range_end;

    int arrival_range_start;
    int arrival_range_end;

    char tempString[100];
    strcpy(tempString, max_proc_range);
    set_ranges(tempString, &max_proc_range_start, &max_proc_range_end);

    strcpy(tempString, exec_range);
    set_ranges(tempString, &exec_range_start, &exec_range_end);

    strcpy(tempString, priority_range);
    set_ranges(tempString, &priority_range_start, &priority_range_end);
    strcpy(tempString, arrival_range);
    set_ranges(tempString, &arrival_range_start, &arrival_range_end);
    int dim = create_random_process_array(resolution_numbers, max_proc_range_start, max_proc_range_end, exec_range_start, exec_range_end, priority_range_start, priority_range_end, arrival_range_start, arrival_range_end);

    char *string = NULL;

    cJSON *options = NULL;
    cJSON *process = NULL;
    cJSON *nested_process = NULL;

    cJSON *quantum = NULL;
    cJSON *algorithm = NULL;

    cJSON *config_file = cJSON_CreateObject();
    if (config_file == NULL)
    {
        goto end;
    }

    options = cJSON_CreateObject();
    if (options == NULL)
    {
        goto end;
    }

    cJSON_AddItemToObject(config_file, "options", options);

    process = cJSON_CreateArray();
    if (process == NULL)
    {
        goto end;
    }

    cJSON_AddItemToObject(config_file, "process", process);

    quantum = cJSON_CreateNumber(ops.quantum);
    if (quantum == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(options, "quantum", quantum);

    algorithm = cJSON_CreateNumber(ops.algorithm);
    if (algorithm == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(options, "algorithm", algorithm);

    for (size_t index = 0; index < dim; ++index)
    {
        nested_process = cJSON_CreateObject();
        if (nested_process == NULL)
        {
            goto end;
        }
        cJSON_AddItemToArray(process, nested_process);

        cJSON_AddItemToObject(nested_process, "arrived_at", cJSON_CreateNumber(resolution_numbers[index].arrived_at));
        cJSON_AddItemToObject(nested_process, "execution_time", cJSON_CreateNumber(resolution_numbers[index].execution_time));
        cJSON_AddItemToObject(nested_process, "priority", cJSON_CreateNumber(resolution_numbers[index].priority));
        cJSON_AddItemToObject(nested_process, "name", cJSON_CreateString(resolution_numbers[index].name));
    }

    string = cJSON_Print(config_file);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print config_file.\n");
    }

end:
    cJSON_Delete(config_file);
    const char *config_file_name = "generated_config.json";
    write_to_config(string);
}
