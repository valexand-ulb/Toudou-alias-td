//
// Created by alex on 9/12/24.
//

#ifndef DEBUG
#define DEBUG 1 // Define DEBUG as 0 if it's not defined
#endif

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define ANSI_RESET "\x1b[0m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_BLUE "\x1b[34m"

// Macro for debug message
#if DEBUG
#define info(msg, ...) printf("[i] [%s] " msg ANSI_RESET"\n", __func__, ##__VA_ARGS__)
#define warn(msg, ...) printf( ANSI_YELLOW"[!] [%s] " msg ANSI_RESET"\n", __func__, ##__VA_ARGS__)
#define okay(msg, ...) printf( ANSI_GREEN "[v] [%s] " msg ANSI_RESET "\n", __func__, ##__VA_ARGS__)
#define err(msg, ...) fprintf(stderr, ANSI_RED "[x] [%s] " msg ANSI_RESET "\n", __func__, ##__VA_ARGS__)
#else
#define info(msg, ...)
#define warn(msg, ...)
#define okay(msg, ...)
#define err(msg, ...)
#endif  // DEBUG

#endif  // LOG_H
