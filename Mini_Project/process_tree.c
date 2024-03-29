#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

void* print_thread_info(void *depth) {
    int current_depth = *((int *)depth);
    pthread_t tid = pthread_self();
    for(int i = 0; i <= current_depth; ++i) {
        printf("\t");
    }
    printf("Thread ID: %lu\n", (unsigned long)tid);
    return NULL;
}

void create_process_and_thread(int depth, int current_depth) {
    if (depth <= 0) {
        return;
    }

    pthread_t thread_id;
    int thread_depth = current_depth + 1; 
    pthread_create(&thread_id, NULL, print_thread_info, &thread_depth);

    pid_t pid = fork();

    if (pid == -1) {
        // If fork() fails
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        wait(NULL); // Wait for the child to finish
        for(int i = 0; i < current_depth; ++i) {
            printf("\t");
        }
        printf("|-- Child PID: %d from Parent PID: %d\n", getpid(), getppid());
        pthread_join(thread_id, NULL); 
        create_process_and_thread(depth - 1, current_depth + 1);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process waits for the child to finish
        wait(NULL);
    }

    if(current_depth == 0) {
        pthread_join(thread_id, NULL); // Ensure the main thread waits for its thread
    }
}

int main() {
    printf("Main Process ID: %d\n", getpid());
    int main_depth = 0; 
    print_thread_info(&main_depth); 
    create_process_and_thread(6, 0); 

    return EXIT_SUCCESS;
}
