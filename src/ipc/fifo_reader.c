#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/sowseelya_fifo"

int main(void)
{
    char buffer[256];
    size_t used = 0;

    if (mkfifo(FIFO_PATH, 0666) == -1) {
        struct stat info;
        if (lstat(FIFO_PATH, &info) == -1 || !S_ISFIFO(info.st_mode)) {
            perror("mkfifo failed");
            return EXIT_FAILURE;
        }
    }

    printf("FIFO reader waiting for a message...\n");
    fflush(stdout);

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open FIFO failed");
        return EXIT_FAILURE;
    }

    while (used < sizeof(buffer) - 1) {
        ssize_t n = read(fd, buffer + used, sizeof(buffer) - 1 - used);
        if (n < 0) {
            perror("read FIFO failed");
            close(fd);
            return EXIT_FAILURE;
        }
        if (n == 0)
            break;
        used += (size_t)n;
    }

    buffer[used] = '\0';
    printf("Reader received: %s\n", buffer);
    close(fd);
    return EXIT_SUCCESS;
}
