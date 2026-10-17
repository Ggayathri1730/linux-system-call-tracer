#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    printf("Parent process started.\n");
    printf("Parent PID: %ld\n", (long)getpid());
    fflush(stdout);

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        printf("\nChild process started.\n");
        printf("Child PID: %ld\n", (long)getpid());
        printf("Parent PID: %ld\n", (long)getppid());
        printf("Child process is exiting.\n");
        fflush(stdout);
        _exit(0);
    }

    printf("\nParent is waiting for child PID: %ld\n", (long)pid);

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child terminated by signal: %d\n", WTERMSIG(status));
    }

    printf("Parent process finished.\n");
    return EXIT_SUCCESS;
}
