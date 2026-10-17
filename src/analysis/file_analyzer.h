#ifndef FILE_ANALYZER_H
#define FILE_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Function prototypes */
void demonstrate_file_lifecycle(const char *filepath);
void demonstrate_file_error_detection(void);

#endif /* FILE_ANALYZER_H */
