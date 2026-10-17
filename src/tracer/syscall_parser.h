#ifndef SYSCALL_PARSER_H
#define SYSCALL_PARSER_H

#define MAX_SYSCALL_TYPES 128
#define MAX_SYSCALL_NAME 64
#define MAX_FAILURES 128
#define MAX_ERROR_TEXT 128

typedef struct {
    char name[MAX_SYSCALL_NAME];
    int count;
} SyscallEntry;

typedef struct {
    char name[MAX_SYSCALL_NAME];
    char error[MAX_ERROR_TEXT];
} SyscallFailure;

typedef struct {
    int total_calls;
    int failed_calls;
    int unique_calls;

    SyscallEntry entries[MAX_SYSCALL_TYPES];

    SyscallFailure failures[MAX_FAILURES];
    int stored_failures;
} SyscallStats;

SyscallStats parse_syscall_trace(const char *filename);

#endif
