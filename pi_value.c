#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

double calculate_pi(long long n, int num_threads) {
    double sum = 0.0;
    double step = 1.0 / (double)n;
    
    omp_set_num_threads(num_threads);
    
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < n; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    double pi = sum * step;
    double end = omp_get_wtime();
    
    (void)pi;
    return end - start;
}

int main() {
    long long data_sizes[] = {1000000, 10000000, 100000000, 1000000000};
    int thread_counts[] = {1, 2, 4, 8, 16};
    int num_sizes = sizeof(data_sizes) / sizeof(data_sizes[0]);
    int num_threads = sizeof(thread_counts) / sizeof(thread_counts[0]);
    
    printf("Data Size,Threads,Time (seconds),Speedup,Efficiency\n");
    
    for (int i = 0; i < num_sizes; i++) {
        long long n = data_sizes[i];
        double single_thread_time = 0.0;
        
        for (int j = 0; j < num_threads; j++) {
            int threads = thread_counts[j];
            double time = calculate_pi(n, threads);
            
            if (threads == 1) {
                single_thread_time = time;
            }
            
            double speedup = single_thread_time / time;
            double efficiency = speedup / threads;
            
            printf("%lld,%d,%.6f,%.6f,%.6f\n", n, threads, time, speedup, efficiency);
        }
        printf("\n");
    }
    
    return 0;
}
