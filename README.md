# Linux System Call Tracer

A menu-driven Linux system programming project that demonstrates system call tracing, process management, inter-process communication (IPC), file and memory analysis, and multithreading concepts.

## Project Overview

The Linux System Call Tracer helps users explore how Linux programs interact with the operating system. It combines a core system call tracing tool with practical demonstrations of important Linux system programming concepts.

## Features

- **System Call Tracing:** Trace program execution using `strace` and generate a system call report.
- **Process Creation:** Demonstrate process creation and execution.
- **Inter-Process Communication:** Demonstrate communication using pipes and FIFOs.
- **Signal Handling:** Demonstrate signal-based process interaction.
- **File I/O Analysis:** Explore file operations.
- **Memory Analysis:** Demonstrate memory-related concepts.
- **Multithreading:** Explore threads, race conditions, and synchronization.
- **Unified Menu:** Access the demonstrations from a single menu-driven program.

## Technologies Used

- C programming
- Linux / Ubuntu
- GCC compiler
- GNU Make
- `strace`
- POSIX process, IPC, and threading APIs

## Requirements

- Ubuntu or a compatible Linux environment
- GCC
- GNU Make
- `strace`
- POSIX threads support

On Ubuntu, install the required tools with:

```bash
sudo apt update
sudo apt install build-essential strace
```

## Build and Run

Clone the repository:

```bash
git clone https://github.com/Ggayathri1730/linux-system-call-tracer.git
cd linux-system-call-tracer
```

Compile the project:

```bash
make
```

Launch the main menu:

```bash
./linux-tracer
```

To run the core tracer directly:

```bash
./core-tracer "ls"
```

The core tracer uses `strace` to capture system call activity. The generated trace report is saved to `reports/trace.txt` when the reports directory is available.

## Project Structure

```text
linux-system-call-tracer/
├── Makefile
├── README.md
├── .gitignore
├── src/
│   ├── main.c
│   ├── main_backup.c
│   ├── analysis/
│   ├── ipc/
│   └── tracer/
├── tests/
└── reports/
```

## Team G-4

| Team Member | Responsibilities |
|---|---|
| Gayathri | Core system call tracer and process monitoring |
| Sai Sowseelya | Process creation and inter-process communication |
| Srithika | File and memory analysis, threading, and synchronization |

## Learning Outcomes

- Understand system calls and how programs interact with the Linux kernel.
- Explore process creation and process communication.
- Learn about pipes, FIFOs, and signal handling.
- Understand file operations, memory concepts, and multithreading.
- Practice compiling and organizing a modular C project using Make.

## Future Enhancements

- Add more detailed system call statistics and visualizations.
- Improve error handling and report formatting.
- Expand automated testing.
- Add support for additional tracing and analysis options.

## License

This project was developed for educational and hackathon purposes.
