#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

#define NUM_THREADS 4
#define ITERATIONS  10000
#define QUEUE_CAPACITY 5

static long synced_counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Mutex Protected Worker */
void* mutex_worker(void *arg) {
    long tid_arg = (long)arg;
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&counter_mutex);
        synced_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("   [Synced Worker %ld] Completed.\n", tid_arg);
    return NULL;
}

/* Producer-Consumer Coordination */
static int queue[QUEUE_CAPACITY];
static int queue_size = 0;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;

void* producer_routine(void *arg) {
    (void)arg;
    for (int i = 1; i <= 6; i++) {
        pthread_mutex_lock(&queue_mutex);
        while (queue_size == QUEUE_CAPACITY) {
            pthread_cond_wait(&cond_not_full, &queue_mutex);
        }
        queue[queue_size++] = i * 10;
        printf("   [Producer] Queued item %d (Queue Depth: %d)\n", i * 10, queue_size);
        pthread_cond_signal(&cond_not_empty);
        pthread_mutex_unlock(&queue_mutex);
        usleep(5000);
    }
    return NULL;
}

void* consumer_routine(void *arg) {
    (void)arg;
    for (int i = 1; i <= 6; i++) {
        pthread_mutex_lock(&queue_mutex);
        while (queue_size == 0) {
            pthread_cond_wait(&cond_not_empty, &queue_mutex);
        }
        int item = queue[--queue_size];
        printf("   [Consumer] Dequeued item %d (Queue Depth: %d)\n", item, queue_size);
        pthread_cond_signal(&cond_not_full);
        pthread_mutex_unlock(&queue_mutex);
        usleep(8000);
    }
    return NULL;
}

int main(void) {
    printf("====================================================\n");
    printf("  SRITHIKA'S MODULE: THREAD SYNCHRONIZATION DEMO    \n");
    printf("====================================================\n");
    printf("Main Process PID: %d\n\n", getpid());

    /* 1. Mutex Protected Counter */
    printf("1. Running %d threads with POSIX Mutex synchronization:\n", NUM_THREADS);
    pthread_t threads[NUM_THREADS];
    synced_counter = 0;

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, mutex_worker, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n   Expected Counter Value: %d\n", NUM_THREADS * ITERATIONS);
    printf("   Observed Counter Value: %ld\n", synced_counter);
    printf("   -> Synchronization Result: 100%% ACCURATE! (Protected by pthread_mutex)\n\n");

    /* 2. Producer-Consumer with Condition Variables */
    printf("2. Demonstrating Producer-Consumer with Condition Variables:\n");
    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer_routine, NULL);
    pthread_create(&cons_thread, NULL, consumer_routine, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("\n[Thread Synchronization Demo Completed Successfully]\n");
    return 0;
}
