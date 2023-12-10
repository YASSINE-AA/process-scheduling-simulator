#ifndef WRITE_H
#define WRITE_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <time.h>
#include <string.h>
#include "../../../../include/types.h"

bool modify_quantum_val(const char* config_filename, int new_value);
int create_random_process_array(process processes[100], int max_proc_range_start, int max_proc_range_end, int exec_range_start, int exec_range_end, int priority_range_start, int priority_range_end, int arrival_range_start, int arrival_range_end);
void write_to_config(const char* config_filename, const char *content);
bool modify_ranges(const char* config_filename, char* proc_range, char* exec_range, char* priority_range, char* arrival_range);
void *generate_config_file(const char* config_filename, options ops, char *max_proc_range, char *exec_range, char *priority_range, char *arrival_range);
#endif