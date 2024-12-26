//
// Created by alex on 11/12/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <time.h>

int expand_tilde(const char* path, char* expanded_path, size_t size);

int manage_time_arg(const char *argument, long int *value);

int timestamp_to_string(time_t timestamp,char *buffer, size_t buffer_size);
#endif //UTILS_H
