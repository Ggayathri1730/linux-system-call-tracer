#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

#include <sched.h>

#define NUM_THREADS 4
#define ITERATIONS  2000

static volatile long unsynced_counter = 0;

void* thread_worker(void *arg) {
    long tid_arg = (long)arg;
    pid_t kernel_tid = syscall(SYS_gettid);
    printf("   [Worker %ld] Started! Linux Kernel Thread TID: %d\n", tid_arg, kernel_tid);

    /* Perform unsynchronized operations on shared variable (Race Condition) */
    for (int i = 0; i < ITERATIONS; i++) {
        long temp = unsynced_counter;
        sched_yield(); // Voluntarily yield CPU to provoke race condition
        unsynced_counter = temp + 1;
    }

    printf("   [Worker %ld] Finished execution.\n", tid_arg);
    return NULL;
}

int main(void) {
    printf("====================================================\n");
    printf("  SRITHIKA'S MODULE: POSIX THREADS & LIFECYCLE DEMO \n");
    printf("====================================================\n");
    printf("Main Process PID: %d, Main Thread TID: %ld\n\n", getpid(), syscall(SYS_gettid));

    pthread_t threads[NUM_THREADS];

    printf("1. Creating %d POSIX threads with pthread_create()...\n", NUM_THREADS);
    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_worker, (void *)i) != 0) {
            perror("pthread_create failed");
        }
    }

    printf("\n2. Main thread waiting for workers via pthread_join()...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n3. Thread Execution Results:\n");
    printf("   Expected Counter Value: %d\n", NUM_THREADS * ITERATIONS);
    printf("   Observed Counter Value: %ld\n", unsynced_counter);
    if (unsynced_counter != NUM_THREADS * ITERATIONS) {
        printf("   -> DATA RACE DETECTED! Lost updates occurred due to lack of synchronization.\n");
    } else {
        printf("   -> No race detected.\n");
    }

    printf("\n[Thread Demo Completed Successfully]\n");
    return 0;
}
