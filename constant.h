//
// Created by alex on 23/12/24.
//

#ifndef CONSTANT_H
#define CONSTANT_H

#define ERROR_LIST "'td list' or 'td list <unsigned>'"
#define ERROR_ADD "'td add error'"
#define ERROR_DONE "'td done <todo_id>'"
#define ERROR_TIME "'td time '"

#define ERROR_FILL "'td fill <unsigned>'"

#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_DAY  (24 * SECONDS_IN_HOUR)
#define SECONDS_IN_WEEK (7 * SECONDS_IN_DAY)
#define SECONDS_IN_MONTH (30 * SECONDS_IN_DAY) // Approximate value
#define SECONDS_IN_YEAR (365 * SECONDS_IN_DAY) // Approximate value

#define TIMESTAMP_NOW (unsigned long) time(NULL)

#endif //CONSTANT_H
