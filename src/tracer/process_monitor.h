#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <sys/types.h>

typedef struct {
    pid_t pid;
    double duration_seconds;
    int exit_status;
} ProcessInfo;

double get_time_seconds(void);

#endif
