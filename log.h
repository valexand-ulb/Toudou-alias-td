//
// Created by alex on 9/12/24.
//

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// Macro for debug message
#define info(msg, ...) printf(" - [i] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf(" - [!] " msg "\n", ##__VA_ARGS__)
#define okay(msg, ...) printf(" - [v] " msg "\n", ##__VA_ARGS__)
#define err(msg, ...) fprintf(stderr, " - [x] " msg "\n", ##__VA_ARGS__)

#endif //LOG_H
