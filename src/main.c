#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int choice;
    char input[256];
    char command[512];

    while (1) {
        printf("\n====================================\n");
        printf("   G-4 LINUX SYSTEM CALL TRACER\n");
        printf("====================================\n");
        printf("1. Core System Call Tracer\n");
        printf("2. Process Creation Demo\n");
        printf("3. Pipe Communication Demo\n");
        printf("4. FIFO Writer\n");
        printf("5. FIFO Reader\n");
        printf("6. Signal Handling Demo\n");
        printf("7. File I/O Analysis\n");
        printf("8. Memory Analysis\n");
        printf("9. Thread Race Condition Demo\n");
        printf("10. Thread Synchronization Demo\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice) {
        case 1:
            printf("Enter a command to trace (example: ls): ");
            if (scanf(" %255[^\n]", input) != 1) {
                printf("Invalid command.\n");
                break;
            }
            snprintf(command, sizeof(command),
                     "./core-tracer \"%s\"", input);
            system(command);
            break;
        case 2: system("./process_demo"); break;
        case 3: system("./pipe_demo"); break;
        case 4: system("./fifo_writer"); break;
        case 5: system("./fifo_reader"); break;
        case 6: system("./signal_demo"); break;
        case 7: system("./file_analyzer"); break;
        case 8: system("./memory_analyzer"); break;
        case 9: system("./thread_demo"); break;
        case 10: system("./sync_demo"); break;
        case 0:
            printf("Exiting tracer. Goodbye!\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}
