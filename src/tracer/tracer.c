#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tracer.h"
#include "process_monitor.h"

int run_tracer(char *command)
{
    double start_time = get_time_seconds();

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        execlp("strace",
               "strace",
               "-f",
               "-o",
               "reports/trace.txt",
               "--",
               "sh",
               "-c",
               command,
               (char *)NULL);

        perror("exec strace failed");
        _exit(127);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid failed");
        return 1;
    }

    double end_time = get_time_seconds();

    printf("Tracer PID       : %ld\n", (long)pid);
    printf("Execution time   : %.6f seconds\n",
           end_time - start_time);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return 1;
}
