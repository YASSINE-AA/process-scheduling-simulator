#ifndef READ_H
#define READ_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <string.h>
#include "../../../../include/types.h"


bool load_settings(char* *proc_range, char* *exec_range, char* *priority_range, char* *arrival_range);
process *read_config_file(const char *filename, int *config_file_size, options* ops);

#endif 