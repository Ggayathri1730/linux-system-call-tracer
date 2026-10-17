#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

static volatile sig_atomic_t signal_received = 0;

static void handle_sigusr1(int signo)
{
    (void)signo;
    signal_received = 1;
}

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_sigusr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction failed");
        return EXIT_FAILURE;
    }

    printf("Process PID: %ld\n", (long)getpid());
    printf("Waiting for SIGUSR1...\n");
    fflush(stdout);

    while (!signal_received) {
        if (pause() == -1 && errno != EINTR) {
            perror("pause failed");
            return EXIT_FAILURE;
        }
    }

    printf("SIGUSR1 received successfully!\n");
    return EXIT_SUCCESS;
}
