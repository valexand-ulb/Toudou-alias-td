#ifndef CONSTANT_H
#define CONSTANT_H

#define ERROR_LIST "'td list' or 'td list <unsigned>' e.g 'td list 5'"
#define ERROR_ADD "'td add \"<content>\" {+,-,}[int]{h,d,w,m,y}' e.g : 'td add \"test\" +1d'"
#define ERROR_DONE "'td done <todo_id>' e.g : td done 2"
#define ERROR_TIME "'td time <todo_id> {+,-,}[int]{h,d,w,m,y}' e.g 'td time -5w'"

#define ERROR_FILL "'td debuf_fill <unsigned>' e.g : 'td debuf_fill 12'"

#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_DAY  (24 * SECONDS_IN_HOUR)
#define SECONDS_IN_WEEK (7 * SECONDS_IN_DAY)
#define SECONDS_IN_MONTH (30 * SECONDS_IN_DAY) // Approximate value
#define SECONDS_IN_YEAR (365 * SECONDS_IN_DAY) // Approximate value

#define TIMESTAMP_NOW (unsigned long) time(NULL)

#endif //CONSTANT_H
