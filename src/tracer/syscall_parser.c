#include <stdio.h>
#include <string.h>

#include "syscall_parser.h"

SyscallStats parse_syscall_trace(const char *filename)
{
    SyscallStats stats = {0};
    char line[4096];

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Unable to open trace file");
        return stats;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *open_paren = strchr(line, '(');
        char *equals = strstr(line, ") = ");

        if (open_paren == NULL || equals == NULL ||
            open_paren > equals) {
            continue;
        }

        stats.total_calls++;
char *name_start = line;

/* Skip leading spaces and tabs. */
while (*name_start == ' ' || *name_start == '\t') {
    name_start++;
}

/* Skip the PID prefix added by strace -f. */
char *space = strchr(name_start, ' ');
if (space != NULL && space < open_paren) {
    int is_pid = 1;

    for (char *p = name_start; p < space; p++) {
        if (*p < '0' || *p > '9') {
            is_pid = 0;
            break;
        }
    }

    if (is_pid) {
        name_start = space + 1;

        while (*name_start == ' ' || *name_start == '\t') {
            name_start++;
        }
    }
}

size_t name_length = (size_t)(open_paren - name_start);
        if (name_length == 0 || name_length >= MAX_SYSCALL_NAME) {
            continue;
        }

        char name[MAX_SYSCALL_NAME];
memcpy(name, name_start, name_length);
        name[name_length] = '\0';

        int index = 0;

        while (index < stats.unique_calls &&
               strcmp(stats.entries[index].name, name) != 0) {
            index++;
        }

        if (index < stats.unique_calls) {
            stats.entries[index].count++;
        } else if (stats.unique_calls < MAX_SYSCALL_TYPES) {
            strcpy(stats.entries[index].name, name);
            stats.entries[index].count = 1;
            stats.unique_calls++;
        }

        char *error = strstr(equals, "= -1 ");

        if (error != NULL) {
            stats.failed_calls++;

            if (stats.stored_failures < MAX_FAILURES) {
                SyscallFailure *failure =
                    &stats.failures[stats.stored_failures];

                strcpy(failure->name, name);

                char *error_end = strchr(error + 5, '\n');
                size_t error_length = error_end
                    ? (size_t)(error_end - (error + 5))
                    : strlen(error + 5);

                if (error_length >= MAX_ERROR_TEXT) {
                    error_length = MAX_ERROR_TEXT - 1;
                }

                memcpy(failure->error, error + 5, error_length);
                failure->error[error_length] = '\0';

                stats.stored_failures++;
            }
        }
    }

    fclose(file);
    return stats;
}
