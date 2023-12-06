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

bool modify_quantum_val(int new_value);
void create_random_process_array(process processes[]);
void write_to_config(const char *content);
void *generate_config_file(options ops, char* exec_range, char* priority_range, char* arrival_range);
#endif