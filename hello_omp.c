#include <stdio.h>
#include <omp.h>

int main() {
    const char* messages[] = {
        "Thread 0 says: Hello, Universe!",
        "Thread 1 says: OpenMP is cool!",
        "Thread 2 says: Parallel computing rocks!",
        "Thread 3 says: Have a great day!",
        "Thread 4 says: Coding is fun!",
        "Thread 5 says: Multithreading FTW!",
        "Thread 6 says: Keep learning!",
        "Thread 7 says: Enjoy parallelism!"
    };
    int num_messages = sizeof(messages) / sizeof(messages[0]);

    #pragma omp parallel num_threads(16)
    {
        int thread_id = omp_get_thread_num();
        if (thread_id < num_messages) {
            printf("%s\n", messages[thread_id]);
        } else {
            printf("Thread %d says: No custom message assigned.\n", thread_id);
        }
    }
    return 0;
}