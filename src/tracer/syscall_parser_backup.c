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

        if (strstr(equals, "= -1 ") != NULL) {
            stats.failed_calls++;
        }

        size_t name_length = (size_t)(open_paren - line);

        if (name_length == 0 || name_length >= MAX_SYSCALL_NAME) {
            continue;
        }

        char name[MAX_SYSCALL_NAME];
        memcpy(name, line, name_length);
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
    }

    fclose(file);
    return stats;
}
