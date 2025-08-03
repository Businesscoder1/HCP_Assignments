#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    // Test configurations
    int sizes[] = {10000, 100000, 1000000, 10000000};  // Array sizes
    int threads[] = {1, 2, 4, 8};                     // Thread counts
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);
    int num_threads = sizeof(threads)/sizeof(threads[0]);
    double s = 2.5;  // Scalar value

    printf("Performance Analysis:\n");
    printf("---------------------------------------------\n");
    printf("%-12s | %-8s | %-12s | %-7s\n", "Data Size", "Threads", "Time (s)", "Speedup");
    printf("---------------------------------------------\n");

    double serial_times[num_sizes];  // Store serial times for speedup calculation
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        double *a = (double*)malloc(n * sizeof(double));
        if (a == NULL) {
            fprintf(stderr, "Memory allocation failed for size %d\n", n);
            continue;
        }

        // Initialize array
        #pragma omp parallel for
        for (int j = 0; j < n; j++) {
            a[j] = j * 1.0;
        }

        // Test different thread counts
        for (int t = 0; t < num_threads; t++) {
            omp_set_num_threads(threads[t]);
            double start, end;

            // Warm-up run
            #pragma omp parallel for
            for (int j = 0; j < n; j++) {
                a[j] = j * 1.0;  // Reset values
            }

            start = omp_get_wtime();
            
            // Vector-scalar addition
            #pragma omp parallel for
            for (int j = 0; j < n; j++) {
                a[j] += s;
            }
            
            end = omp_get_wtime();
            double elapsed = end - start;

            // Calculate speedup relative to serial (1-thread) version
            double speedup = (t == 0) ? 1.0 : serial_times[i] / elapsed;
            
            if (t == 0) {
                serial_times[i] = elapsed;  // Store serial time for this size
            }

            printf("%-12d | %-8d | %-12.6f | %-7.2f\n", 
                   n, threads[t], elapsed, speedup);
        }
        free(a);
        printf("---------------------------------------------\n");
    }
    return 0;
}