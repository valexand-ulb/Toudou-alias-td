//
// Created by alex on 11/12/24.
//

#include "utils.h"

#include <time.h>

#include "log.h"

int timestamp_to_string(time_t timestamp, char *buffer, size_t buffer_size) {
    // Convert timestamp to local time
    struct tm *tm_info = localtime(&timestamp);
    if (tm_info == NULL) {
        fprintf(stderr, "localtime failed\n");
        return 1;
    }

    // Format the time into the provided buffer
    if (strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info) == 0) {
        fprintf(stderr, "strftime failed: Buffer might be too small\n");
        return 1;
    }

    return 0; // Success
}