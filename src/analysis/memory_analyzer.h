#ifndef MEMORY_ANALYZER_H
#define MEMORY_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/* Structure to hold snapshot of process memory metrics */
typedef struct {
    unsigned long vm_size_kb;
    unsigned long vm_rss_kb;
    unsigned long vm_data_kb;
    unsigned long vm_stk_kb;
} ProcessMemorySnapshot;

/* Function prototypes */
void print_memory_status(const char *stage_name);
ProcessMemorySnapshot capture_memory_snapshot(pid_t pid);
void demonstrate_memory_syscalls(void);
void analyze_external_process_memory(pid_t pid);

#endif /* MEMORY_ANALYZER_H */
