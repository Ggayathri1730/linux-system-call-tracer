#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "file_analyzer.h"

void demonstrate_file_lifecycle(const char *filepath) {
    printf("1. [File Creation & Write] Opening '%s' with O_CREAT | O_WRONLY | O_TRUNC...\n", filepath);
    int fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        return;
    }

    const char *payload = "Linux System Call Tracer - File I/O Analysis Module\nAuthor: Srithika (CO-4 & CO-5)\n";
    ssize_t bytes_written = write(fd, payload, strlen(payload));
    printf("   Written %zd bytes to fd %d.\n", bytes_written, fd);
    close(fd);

    printf("\n2. [File Stat, Seek & Read] Reopening '%s' with O_RDONLY...\n", filepath);
    fd = open(filepath, O_RDONLY);
    if (fd >= 0) {
        struct stat st;
        if (fstat(fd, &st) == 0) {
            printf("   fstat: File size = %ld bytes, Inode = %lu, Mode = 0%o\n", st.st_size, st.st_ino, st.st_mode & 0777);
        }

        off_t offset = lseek(fd, 6, SEEK_SET);
        printf("   lseek: Offset positioned at byte %ld.\n", offset);

        char buf[64];
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
        printf("   read: Retrieved %zd bytes: \"%s\"\n", bytes_read, buf);
        close(fd);
    }
}

void demonstrate_file_error_detection(void) {
    printf("\n3. [Error Detection Scenarios]\n");

    /* Case A: ENOENT */
    const char *non_existent = "/tmp/srithika_missing_404.txt";
    printf("   [Error Test 1] Attempting to open non-existent file '%s'...\n", non_existent);
    int fd1 = open(non_existent, O_RDONLY);
    if (fd1 < 0) {
        printf("   -> Intercepted expected failure: errno = %d (%s)\n", errno, strerror(errno));
    } else {
        close(fd1);
    }

    /* Case B: EACCES */
    const char *ro_file = "/tmp/srithika_readonly.txt";
    printf("   [Error Test 2] Attempting to open read-only file with O_WRONLY...\n");
    int ro_fd = open(ro_file, O_CREAT | O_WRONLY | O_TRUNC, 0444);
    if (ro_fd >= 0) {
        if (write(ro_fd, "readonly_data", 13) < 0) {
            perror("write ro");
        }
        close(ro_fd);
        chmod(ro_file, 0444);

        int fd2 = open(ro_file, O_WRONLY);
        if (fd2 < 0) {
            printf("   -> Intercepted expected failure: errno = %d (%s)\n", errno, strerror(errno));
        } else {
            close(fd2);
        }
        unlink(ro_file);
    }

    /* Case C: EBADF */
    const char *sample_file = "/tmp/srithika_temp_ebadf.txt";
    printf("   [Error Test 3] Attempting to write into read-only descriptor...\n");
    int fd3 = open(sample_file, O_CREAT | O_RDWR, 0644);
    if (fd3 >= 0) {
        close(fd3); // close descriptor
        ssize_t res = write(fd3, "invalid", 7); // write to closed fd
        if (res < 0) {
            printf("   -> Intercepted expected failure: errno = %d (%s)\n", errno, strerror(errno));
        }
        unlink(sample_file);
    }

    /* Case D: EISDIR */
    printf("   [Error Test 4] Attempting to open directory '/tmp' with O_WRONLY...\n");
    int fd4 = open("/tmp", O_WRONLY);
    if (fd4 < 0) {
        printf("   -> Intercepted expected failure: errno = %d (%s)\n", errno, strerror(errno));
    } else {
        close(fd4);
    }
}

int main(void) {
    printf("====================================================\n");
    printf("  SRITHIKA'S MODULE: FILE OPERATIONS & ERROR DETECTOR\n");
    printf("====================================================\n");
    printf("Process PID: %d\n\n", getpid());

    const char *test_path = "/tmp/srithika_file_test.txt";
    demonstrate_file_lifecycle(test_path);
    demonstrate_file_error_detection();

    unlink(test_path);
    printf("\n[File Analysis Completed Successfully]\n");
    return 0;
}
