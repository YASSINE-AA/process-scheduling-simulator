#ifndef READ_H
#define READ_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cjson/cJSON.h>

#include "../../../include/types.h"

process *read_config_file(const char *filename, int *config_file_size);

#endif 