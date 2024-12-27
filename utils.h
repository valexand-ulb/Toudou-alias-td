#ifndef UTILS_H
#define UTILS_H

int expand_tilde(const char* path, char* expanded_path, size_t size);

int manage_time_arg(const char *argument, long int *value);

#endif //UTILS_H
