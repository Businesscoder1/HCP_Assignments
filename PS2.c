#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_add(int **a, int **b, int **c, int size, int threads) {
    omp_set_num_threads(threads);
    
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};
    int threads[] = {1, 2, 4, 8};
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);
    int num_threads = sizeof(threads)/sizeof(threads[0]);
    
    printf("Matrix Size,Threads,Time (s),Speedup,Efficiency\n");
    
    for (int s = 0; s < num_sizes; s++) {
        int size = sizes[s];
        
        int **a = malloc(size * sizeof(int*));
        int **b = malloc(size * sizeof(int*));
        int **c = malloc(size * sizeof(int*));
        
        for (int i = 0; i < size; i++) {
            a[i] = malloc(size * sizeof(int));
            b[i] = malloc(size * sizeof(int));
            c[i] = malloc(size * sizeof(int));
            
            #pragma omp parallel for schedule(auto)
            for (int j = 0; j < size; j++) {
                a[i][j] = rand() % 100;
                b[i][j] = rand() % 100;
            }
        }
        
        double base_time = 0;
        
        for (int t = 0; t < num_threads; t++) {
            double start = omp_get_wtime();
            matrix_add(a, b, c, size, threads[t]);
            double end = omp_get_wtime();
            double elapsed = end - start;
            
            if (t == 0) base_time = elapsed;
            
            double speedup = base_time / elapsed;
            double efficiency = speedup / threads[t];
            
            printf("%d,%d,%.6f,%.6f,%.6f\n", 
                   size, threads[t], elapsed, speedup, efficiency);
        }
        
        for (int i = 0; i < size; i++) {
            free(a[i]);
            free(b[i]);
            free(c[i]);
        }
        free(a);
        free(b);
        free(c);
    }
    
    return 0;
}