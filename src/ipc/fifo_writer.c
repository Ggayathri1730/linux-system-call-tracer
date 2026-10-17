#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/sowseelya_fifo"

int main(void)
{
    const char message[] = "Hello through a named FIFO!";
    size_t length = strlen(message);
    size_t total_written = 0;
    struct stat info;

    if (lstat(FIFO_PATH, &info) == -1 || !S_ISFIFO(info.st_mode)) {
        fprintf(stderr, "FIFO does not exist. Start the reader first.\n");
        return EXIT_FAILURE;
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open FIFO failed");
        return EXIT_FAILURE;
    }

    while (total_written < length) {
        ssize_t n = write(fd, message + total_written, length - total_written);
        if (n < 0) {
            perror("write FIFO failed");
            close(fd);
            return EXIT_FAILURE;
        }
        if (n == 0) {
            fprintf(stderr, "write returned zero bytes\n");
            close(fd);
            return EXIT_FAILURE;
        }
        total_written += (size_t)n;
    }

    printf("Writer sent: %s\n", message);
    close(fd);
    return EXIT_SUCCESS;
}
