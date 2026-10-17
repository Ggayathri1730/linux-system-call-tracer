#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "memory_analyzer.h"

void print_memory_status(const char *stage_name) {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return;

    char line[256];
    printf("\n--- Memory Metric [%s] ---\n", stage_name);
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "VmSize:", 7) == 0 ||
            strncmp(line, "VmRSS:", 6) == 0 ||
            strncmp(line, "VmData:", 7) == 0 ||
            strncmp(line, "VmStk:", 6) == 0) {
            printf("  %s", line);
        }
    }
    fclose(f);
}

ProcessMemorySnapshot capture_memory_snapshot(pid_t pid) {
    ProcessMemorySnapshot snap = {0, 0, 0, 0};
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *f = fopen(path, "r");
    if (!f) return snap;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line + 7, "%lu", &snap.vm_size_kb);
        } else if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line + 6, "%lu", &snap.vm_rss_kb);
        } else if (strncmp(line, "VmData:", 7) == 0) {
            sscanf(line + 7, "%lu", &snap.vm_data_kb);
        } else if (strncmp(line, "VmStk:", 6) == 0) {
            sscanf(line + 6, "%lu", &snap.vm_stk_kb);
        }
    }
    fclose(f);
    return snap;
}

void demonstrate_memory_syscalls(void) {
    printf("Process PID: %d\n", getpid());
    print_memory_status("Initial Baseline");

    /* 1. Dynamic Heap Allocation via malloc (triggers sys_brk) */
    printf("\n1. Allocating 128 KB via malloc()...\n");
    size_t heap_size = 128 * 1024;
    char *heap_buf = (char *)malloc(heap_size);
    if (heap_buf) {
        memset(heap_buf, 0xAA, heap_size);
        printf("   Touched 128 KB memory on heap at %p\n", (void *)heap_buf);
    }
    print_memory_status("After Heap Allocation");

    /* 2. Direct Anonymous Memory Mapping via mmap (triggers sys_mmap) */
    printf("\n2. Allocating 4 MB anonymous memory via mmap()...\n");
    size_t mmap_size = 4 * 1024 * 1024;
    void *mapped_mem = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapped_mem == MAP_FAILED) {
        perror("mmap failed");
    } else {
        printf("   Successfully mmapped 4 MB at %p\n", mapped_mem);
        memset(mapped_mem, 0x55, mmap_size); // Force page fault into physical RAM
        print_memory_status("After 4 MB mmap (touched)");

        /* 3. Memory Protection Change via mprotect (triggers sys_mprotect) */
        printf("\n3. Setting memory protection to READ-ONLY via mprotect()...\n");
        if (mprotect(mapped_mem, mmap_size, PROT_READ) == 0) {
            printf("   Protection updated to PROT_READ successfully.\n");
        } else {
            perror("mprotect failed");
        }

        /* 4. Deallocation via munmap (triggers sys_munmap) */
        printf("\n4. Deallocating mapped memory via munmap()...\n");
        if (munmap(mapped_mem, mmap_size) == 0) {
            printf("   4 MB memory unmapped successfully.\n");
        } else {
            perror("munmap failed");
        }
        print_memory_status("After munmap");
    }

    /* 5. Memory Leak Simulation */
    printf("\n5. Simulating unfreed memory leak (512 KB)...\n");
    void *leak = malloc(512 * 1024);
    if (leak) {
        memset(leak, 0xCC, 512 * 1024);
        printf("   Allocated 512 KB chunk at %p without free().\n", leak);
    }

    if (heap_buf) free(heap_buf);
    print_memory_status("Final State (Leak Retained)");
    printf("\n[Memory Analysis Completed Successfully]\n");
}

int main(int argc, char *argv[]) {
    printf("====================================================\n");
    printf("  SRITHIKA'S MODULE: MEMORY USAGE & SYSCALL ANALYZER \n");
    printf("====================================================\n");

    if (argc > 1) {
        pid_t target = (pid_t)atoi(argv[1]);
        printf("Analyzing external process PID: %d\n", target);
        ProcessMemorySnapshot snap = capture_memory_snapshot(target);
        printf("  VmSize: %lu kB\n", snap.vm_size_kb);
        printf("  VmRSS : %lu kB\n", snap.vm_rss_kb);
        printf("  VmData: %lu kB\n", snap.vm_data_kb);
        printf("  VmStk : %lu kB\n", snap.vm_stk_kb);
    } else {
        demonstrate_memory_syscalls();
    }

    return 0;
}
