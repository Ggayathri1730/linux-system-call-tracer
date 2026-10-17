
CC = gcc
CFLAGS = -Wall -Wextra -I src

# Main menu and original core tracer
TARGET = linux-tracer
CORE_TARGET = core-tracer
MENU_SOURCE = src/main.c

TRACER_SOURCES = src/tracer/tracer.c \
                 src/tracer/syscall_parser.c \
                 src/tracer/process_monitor.c

# Teammate demo executables
IPC_TARGETS = process_demo pipe_demo fifo_writer fifo_reader signal_demo
ANALYSIS_TARGETS = file_analyzer memory_analyzer thread_demo sync_demo

all: $(TARGET) $(CORE_TARGET) $(IPC_TARGETS) $(ANALYSIS_TARGETS)

# Unified menu
$(TARGET): $(MENU_SOURCE)
	$(CC) $(CFLAGS) $(MENU_SOURCE) -o $(TARGET)

# Original system call tracer
$(CORE_TARGET): src/main_backup.c $(TRACER_SOURCES)
	$(CC) $(CFLAGS) src/main_backup.c $(TRACER_SOURCES) -o $(CORE_TARGET)

# Process and IPC demos
process_demo: src/ipc/process_demo.c
	$(CC) $(CFLAGS) $< -o $@

pipe_demo: src/ipc/pipe_demo.c
	$(CC) $(CFLAGS) $< -o $@

fifo_writer: src/ipc/fifo_writer.c
	$(CC) $(CFLAGS) $< -o $@

fifo_reader: src/ipc/fifo_reader.c
	$(CC) $(CFLAGS) $< -o $@

signal_demo: src/ipc/signal_demo.c
	$(CC) $(CFLAGS) $< -o $@

# File and memory analysis demos
file_analyzer: src/analysis/file_analyzer.c
	$(CC) $(CFLAGS) $< -o $@

memory_analyzer: src/analysis/memory_analyzer.c
	$(CC) $(CFLAGS) $< -o $@

# Thread demos
thread_demo: src/analysis/thread_demo.c
	$(CC) $(CFLAGS) $< -pthread -o $@

sync_demo: src/analysis/sync_demo.c
	$(CC) $(CFLAGS) $< -pthread -o $@

clean:
	rm -f $(TARGET) $(CORE_TARGET) $(IPC_TARGETS) $(ANALYSIS_TARGETS)

.PHONY: all clean
