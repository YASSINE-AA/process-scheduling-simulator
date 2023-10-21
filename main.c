#include <stdlib.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>
#include "./types.h"
// My headers
#include "./utils/algorithms/queue_data_struct.c"
#include "./utils/algorithms/useful.c"
#include "./utils/algorithms/round_robin.c"
#include "./utils/algorithms/FIFO.c"
#include "./utils/generation/generation.c"

// Config filename
const char* filename = "generated_config.json";
int config_file_size = 0;

process* read_config_file(const char* filename) {
    process* process_array = malloc(12 * sizeof(process)); 
    if (process_array == NULL) {
        printf("Error: memory allocation failed\n");
        return NULL;
    }

    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        free(process_array); 
        return NULL;
    }


    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* config = (char*)malloc(file_size + 1);
    if (config == NULL) {
        printf("Error: memory allocation failed\n");
        fclose(fp);
        free(process_array);
        return NULL;
    }

    fread(config, 1, file_size, fp);
    config[file_size] = '\0';

    fclose(fp);

    cJSON* config_json = cJSON_Parse(config);
    if (config_json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(config);
        free(process_array);
        return NULL;
    }

    const cJSON* nested_process = NULL;
    const cJSON* process_list = NULL;
    int status = 0;

    process_list = cJSON_GetObjectItemCaseSensitive(config_json, "process");
    cJSON_ArrayForEach(nested_process, process_list) {
        cJSON* arrived_at = cJSON_GetObjectItemCaseSensitive(nested_process, "arrived_at");
        cJSON* execution_time = cJSON_GetObjectItemCaseSensitive(nested_process, "execution_time");
        cJSON* priority = cJSON_GetObjectItemCaseSensitive(nested_process, "priority");
        cJSON* name = cJSON_GetObjectItemCaseSensitive(nested_process, "name");

        if (!cJSON_IsNumber(arrived_at) || !cJSON_IsNumber(execution_time) || !cJSON_IsNumber(priority) || !cJSON_IsString(name)) {
            status = 0;
            break;
        } else {
            process proc;
            proc.arrived_at = arrived_at->valueint;
            proc.execution_time = execution_time->valueint;
            proc.priority = priority->valueint;
            strcpy(proc.name, name->valuestring);
            process_array[config_file_size] = proc;
            config_file_size++;
            status = 1;
        }
    }

    cJSON_Delete(config_json);
    free(config);
    return process_array;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please input the config file.\n");
    } else {
        options ops;
        ops.algorithm = 10;
        ops.quantum = 3;
       if (strcmp(argv[1], "G") == 0 || strcmp(argv[1], "g") == 0) {
         generate_config_file(ops);
         return 0;
        } 
        // Lire fichier configuration
        process* proc_head = read_config_file(argv[1]);
        /*if (proc_head != NULL) {
            for (int i = 0; i < config_file_size; i++) {
                printf("Arrived At: %d\n", proc_head[i].arrived_at);
                printf("Execution Time: %d\n", proc_head[i].execution_time);
                printf("Priority: %d\n", proc_head[i].priority);
                printf("Name: %s\n", proc_head[i].name);
            }
            free(proc_head);
        }*/

        //round_robin_output(ops.quantum, proc_head, config_file_size);
 
        get_fifo_output(proc_head, config_file_size);
        
    }
    return 0;
}