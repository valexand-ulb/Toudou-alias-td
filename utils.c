//
// Created by alex on 11/12/24.
//

#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constant.h"
#include "log.h"


int calculate_timestamp(int multiplicator, char unit, long int *value)
{
    int additional_seconds = 0;

    switch (unit) {
    case 'h':
        additional_seconds = multiplicator * SECONDS_IN_HOUR;
        break;
    case 'd':
        additional_seconds = multiplicator * SECONDS_IN_DAY;
        break;
    case 'w':
        additional_seconds = multiplicator * SECONDS_IN_WEEK;
        break;
    case 'm':
        additional_seconds = multiplicator * SECONDS_IN_MONTH;
        break;
    case 'y':
        additional_seconds = multiplicator * SECONDS_IN_YEAR;
        break;
    default:
        err("Error: Invalid unit '%c'\n", unit);
        return -1;
    }

    *value += additional_seconds;

    return 0;
}

int expand_tilde(const char* path, char* expanded_path, size_t size)
{
    if (path[0] == '~')
    {
        const char* home = getenv("HOME");
        if (home)
        {
            snprintf(expanded_path, size, "%s%s", home, path + 1);
            return 1;
        }
        err("Error : Unable to resolve '~'.");
        return 0;
    }
    strncpy(expanded_path, path, size);
    return 0;
}

int manage_time_arg(const char *arg, long int *value)
{
    char *endptr;
    long numeric_part = 0;
    char unit = '0';

    numeric_part = strtol(arg, &endptr, 10); // Parse integer at the start of the string
    if (endptr == arg) {
        err("Error: No numeric value found in argument '%s'\n", arg);
        return -1;
    }

    unit = *endptr;
    if (unit != 'h' && unit != 'd' && unit != 'w' && unit != 'm' && unit != 'y') {
        err("Error: Unknown unit '%c' in argument '%s'\n",unit, arg);
        return -1;
    }

    info("Argument '%s' is %ld and %c\n", arg, numeric_part, unit);

    if (calculate_timestamp(numeric_part, unit, value) != 0)
    {
        err("Error: Unable to parse argument '%s'\n", arg);
        return -1;
    }

    info("Value is now %d\n", *value);

    return 0;
}

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



