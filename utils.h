//
// Created by alex on 11/12/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <time.h>

int timestamp_to_string(time_t timestamp,char *buffer, size_t buffer_size);
int expand_tilde(const char* path, char* expanded_path, size_t size);
#endif //UTILS_H
