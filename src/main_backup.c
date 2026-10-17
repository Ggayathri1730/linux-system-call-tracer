#include <stdio.h>

#include "tracer/tracer.h"
#include "tracer/syscall_parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: ./linux-tracer \"command\"\n");
        printf("Example: ./linux-tracer \"ls\"\n");
        return 1;
    }

    printf("=== G-4 Linux System Call Tracer ===\n");
    printf("Command: %s\n", argv[1]);
    printf("Starting system call tracing...\n\n");

    int result = run_tracer(argv[1]);

    if (result != 0) {
        printf("\nCommand failed with status: %d\n", result);
    } else {
        printf("\nCommand completed successfully.\n");
    }

    SyscallStats stats = parse_syscall_trace("reports/trace.txt");

    printf("\n===== System Call Summary =====\n");
    printf("Total system calls : %d\n", stats.total_calls);
    printf("Failed system calls: %d\n", stats.failed_calls);
    printf("Successful calls   : %d\n",
           stats.total_calls - stats.failed_calls);

    printf("\n===== Calls by Name =====\n");

    for (int i = 0; i < stats.unique_calls; i++) {
        printf("%-20s : %d\n",
               stats.entries[i].name,
               stats.entries[i].count);
    }

    printf("\n===== Top 5 System Calls =====\n");

    int used[MAX_SYSCALL_TYPES] = {0};
    int limit = stats.unique_calls < 5
        ? stats.unique_calls
        : 5;

    for (int rank = 0; rank < limit; rank++) {
        int max_index = -1;

        for (int i = 0; i < stats.unique_calls; i++) {
            if (!used[i] &&
                (max_index == -1 ||
                 stats.entries[i].count >
                 stats.entries[max_index].count)) {
                max_index = i;
            }
        }

        if (max_index == -1) {
            break;
        }

        printf("%d. %-20s : %d calls\n",
               rank + 1,
               stats.entries[max_index].name,
               stats.entries[max_index].count);

        used[max_index] = 1;
    }

    printf("\n===== Failed System Calls =====\n");

    if (stats.stored_failures == 0) {
        printf("No failed system calls recorded.\n");
    } else {
        for (int i = 0; i < stats.stored_failures; i++) {
            printf("%-20s : %s\n",
                   stats.failures[i].name,
                   stats.failures[i].error);
        }
    }

    printf("\nTrace saved to reports/trace.txt\n");

    return 0;
}
