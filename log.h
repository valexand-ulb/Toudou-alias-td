//
// Created by alex on 9/12/24.
//

#ifndef DEBUG
#define DEBUG 1  // Define DEBUG as 0 if it's not defined
#endif

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// Macro for debug message
#if DEBUG
#define info(msg, ...) printf(" - [i] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf(" - [!] " msg "\n", ##__VA_ARGS__)
#define okay(msg, ...) printf(" - [v] " msg "\n", ##__VA_ARGS__)
#define err(msg, ...) fprintf(stderr, " - [x] " msg "\n", ##__VA_ARGS__)
#else
#define info(msg, ...)
#define warn(msg, ...)
#define okay(msg, ...)
#define err(msg, ...)
#endif  // DEBUG

#endif  // LOG_H
