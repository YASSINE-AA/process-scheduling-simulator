#include <cjson/cJSON.h>
#include <stdio.h>
#include "./utils/generation/generation.c"
#include <string.h>

//Config filename
const char* filename = "generated_config.json";

int read_config_file(const char *filename) {
    process process_array[12];
    int j = 0;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    // Determine the size of the file
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *config = (char *)malloc(file_size + 1);
    if (config == NULL) {
        printf("Error: memory allocation failed\n");
        fclose(fp);
        return 1;
    }

    fread(config, 1, file_size, fp);
    config[file_size] = '\0';

    fclose(fp);

    cJSON *config_json = cJSON_Parse(config);
    if (config_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(config);
        cJSON_Delete(config_json);
        return 0;
    }

    const cJSON *nested_process = NULL;
    const cJSON *process_list = NULL;
    int status = 0;

    process_list = cJSON_GetObjectItemCaseSensitive(config_json, "process");
    cJSON_ArrayForEach(nested_process, process_list) {
        cJSON *arrived_at = cJSON_GetObjectItemCaseSensitive(nested_process, "arrived_at");
        cJSON *execution_time = cJSON_GetObjectItemCaseSensitive(nested_process, "execution_time");
        cJSON *priority = cJSON_GetObjectItemCaseSensitive(nested_process, "priority");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(nested_process, "name");

        if (!cJSON_IsNumber(arrived_at) || !cJSON_IsNumber(execution_time) || !cJSON_IsNumber(priority) || !cJSON_IsString(name)) {
            status = 0;
            break;
        } else {
            process proc;
            proc.arrived_at = arrived_at->valueint;
            proc.execution_time = execution_time->valueint;
            proc.priority = priority->valueint;
            for (int i = 0; i < strlen(name->valuestring); i++) {
                proc.name[i] = name->valuestring[i];
            }
            process_array[j] = proc;
            j++;
            status = 1;
        }
    }

    for (int i = 0; i < j; i++) { 
        printf("Arrived At: %d\n", process_array[i].arrived_at);
        printf("Execution Time: %d\n", process_array[i].execution_time);
        printf("Priority: %d\n", process_array[i].priority);
        printf("Name: %s\n", process_array[i].name);
    }

    cJSON_Delete(config_json);
    free(config);
    return status;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {printf("Please input the config file.\n");}
    else  {
        // Sample data
        //? TODO: Make the user capable of choosing.
        options ops;
        ops.algorithm = 10;
        ops.quantum = 3;
        generate_config_file(ops);
        read_config_file(filename);
    }

   
    return 0;
}