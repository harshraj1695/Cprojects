#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Simple handler for division-by-zero
void handle_sigfpe(int signum) {
    printf(" Error: Division by zero detected (signal %d)\n", signum);
    exit(1);  // Exit after handling
}

int main() {
    signal(SIGFPE, handle_sigfpe);  // Register signal handler

    int a = 10, b = 0;
    int result = a / b;  // Triggers SIGFPE

    printf("Result: %d\n", result);
    return 0;
}
