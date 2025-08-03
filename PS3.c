#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define VECTOR_SIZE 200

int main() {
    int vector[VECTOR_SIZE];
    int scalar = 5;
    int chunk_sizes[] = {1, 10, 20, 50, 100};
    int num_chunks = sizeof(chunk_sizes)/sizeof(chunk_sizes[0]);
    
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = i;
    }

    printf("Schedule Type,Chunk Size,Time (seconds)\n");
    
    for (int c = 0; c < num_chunks; c++) {
        int chunk = chunk_sizes[c];
        double start = omp_get_wtime();
        
        #pragma omp parallel for schedule(static, chunk)
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] += scalar;
        }
        
        double end = omp_get_wtime();
        printf("static,%d,%.6f\n", chunk, end - start);
    }
    
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = i;
    }
    
    for (int c = 0; c < num_chunks; c++) {
        int chunk = chunk_sizes[c];
        double start = omp_get_wtime();
        
        #pragma omp parallel for schedule(dynamic, chunk)
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] += scalar;
        }
        
        double end = omp_get_wtime();
        printf("dynamic,%d,%.6f\n", chunk, end - start);
    }
    
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = i;
    }
    
    printf("\nNowait Demonstration:\n");
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] += scalar;
            printf("Thread %d completed element %d\n", omp_get_thread_num(), i);
        }
        
        #pragma omp single
        {
            printf("---> Nowait allows this message to appear mid-calculation!\n");
        }
    }
    
    printf("\nFinal results (first 10 elements):\n");
    for (int i = 0; i < 10; i++) {
        printf("vector[%d] = %d\n", i, vector[i]);
    }
    
    return 0;
}