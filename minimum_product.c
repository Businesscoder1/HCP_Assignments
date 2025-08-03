#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Comparator function for ascending order
int asc_comp(const void *a, const void *b) {
    return (*(double*)a > *(double*)b) ? 1 : -1;
}

// Comparator function for descending order
int desc_comp(const void *a, const void *b) {
    return (*(double*)a < *(double*)b) ? 1 : -1;
}

int main() {
    int n = 1000000;  // Vector size
    double *a = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    
    // Initialize vectors with random values
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        a[i] = (double)rand() / RAND_MAX;
        b[i] = (double)rand() / RAND_MAX;
    }
    
    // Sort vectors for minimum scalar product
    qsort(a, n, sizeof(double), asc_comp);
    qsort(b, n, sizeof(double), desc_comp);
    
    double min_product = 0.0;
    double start = omp_get_wtime();
    
    // Compute scalar product with OpenMP clauses
    #pragma omp parallel for reduction(+:min_product) schedule(dynamic, 1000)
    for (int i = 0; i < n; i++) {
        min_product += a[i] * b[i];
    }
    
    double end = omp_get_wtime();
    
    printf("Minimum scalar product: %.4f\n", min_product);
    printf("Execution time: %.6f seconds\n", end - start);
    
    free(a);
    free(b);
    return 0;
}