#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    pid_t pid;
    int status;
    const char message[] = "Hello from the parent process!";
    char buffer[100];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        close(fd[0]);
        close(fd[1]);
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        close(fd[1]);

        ssize_t bytes_read = read(fd[0], buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("read failed");
            close(fd[0]);
            _exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("Child received: %s\n", buffer);
        fflush(stdout);
        close(fd[0]);
        _exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    size_t length = strlen(message);
    size_t total_written = 0;

    while (total_written < length) {
        ssize_t bytes_written = write(fd[1], message + total_written,
                                      length - total_written);
        if (bytes_written < 0) {
            perror("write failed");
            close(fd[1]);
            waitpid(pid, &status, 0);
            return EXIT_FAILURE;
        }
        if (bytes_written == 0) {
            fprintf(stderr, "write returned zero bytes\n");
            close(fd[1]);
            waitpid(pid, &status, 0);
            return EXIT_FAILURE;
        }
        total_written += (size_t)bytes_written;
    }

    printf("Parent sent a message through the pipe.\n");
    close(fd[1]);

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
        fprintf(stderr, "Child process failed.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
