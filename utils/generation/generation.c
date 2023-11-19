#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int rand_dim = 20;

void create_random_process_array(process processes[rand_dim]) {
    srand(time(NULL)); 
    for (int i = 0; i < rand_dim; i++) {
        processes[i].arrived_at = rand() % 45 + 1;
        processes[i].execution_time = rand() % 10 + 1;
        processes[i].priority = rand() % 10 + 1;
        snprintf(processes[i].name, sizeof(processes[i].name), "p%d", i + 1);
    }
}

void write_to_config(const char* content) {
    FILE *fptr = fopen("generated_config.json", "w");
    fprintf(fptr,"%s",content);
    fclose(fptr);
}

void *generate_config_file(options ops) {
    srand(time(NULL)); 
    rand_dim = rand() % 10 + 2;
    
    process resolution_numbers[rand_dim];
    create_random_process_array(resolution_numbers);

    char *string = NULL;

    // Main attributes
    cJSON *options = NULL;
    cJSON *process = NULL;
    cJSON *nested_process = NULL;

    // Options
    cJSON *quantum = NULL;
    cJSON *algorithm = NULL;

    cJSON *config_file = cJSON_CreateObject();
    if (config_file == NULL) {
        goto end;
    }

    // Options Section
    options = cJSON_CreateObject();
    if (options == NULL) {
        goto end;
    }

    cJSON_AddItemToObject(config_file, "options", options);

    process = cJSON_CreateArray();
    if (process == NULL) {
        goto end;
    }

    cJSON_AddItemToObject(config_file, "process", process);

    quantum = cJSON_CreateNumber(ops.quantum);
    if (quantum == NULL) {
        goto end;
    }
    cJSON_AddItemToObject(options, "quantum", quantum);

    algorithm = cJSON_CreateNumber(ops.algorithm);
    if (algorithm == NULL) {
        goto end;
    }
    cJSON_AddItemToObject(options, "algorithm", algorithm);

    for (size_t index = 0; index < rand_dim; ++index) {
        nested_process = cJSON_CreateObject();
        if (nested_process == NULL) {
            goto end;
        }
        cJSON_AddItemToArray(process, nested_process);

        cJSON_AddItemToObject(nested_process, "arrived_at", cJSON_CreateNumber(resolution_numbers[index].arrived_at));
        cJSON_AddItemToObject(nested_process, "execution_time", cJSON_CreateNumber(resolution_numbers[index].execution_time));
        cJSON_AddItemToObject(nested_process, "priority", cJSON_CreateNumber(resolution_numbers[index].priority));
        cJSON_AddItemToObject(nested_process, "name", cJSON_CreateString(resolution_numbers[index].name));
    }

    string = cJSON_Print(config_file);
    if (string == NULL) {
        fprintf(stderr, "Failed to print config_file.\n");
    }

end:
    cJSON_Delete(config_file);
    const char* config_file_name = "generated_config.json";
    write_to_config(string);
}
