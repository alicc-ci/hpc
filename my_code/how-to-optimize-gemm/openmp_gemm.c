#include <stdio.h>
#include <omp.h>
#include "defs.h"

void MY_MMult_openmp(int m, int n, int k, double *A, int lda, double *B, int ldb, double *C, int ldc) {
    int i, j, p;
    double sum;

    // Parallelized outer loops with OpenMP
    #pragma omp parallel for private(i, j, p, sum) shared(A, B, C)
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            sum = 0.0;
            for (p = 0; p < k; p++) {
                sum += A[i * lda + p] * B[p * ldb + j];
            }
            C[i * ldc + j] = sum;
        }
    }
}
