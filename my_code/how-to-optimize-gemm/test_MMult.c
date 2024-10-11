#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include "defs.h"
#include <sys/time.h>
// #include "MY_MMult.c"
// #include "blas_cal.c"
// #include "openmp_gemm.c"

#define NUM_THREADS 4  // 定义线程数量

typedef struct {
    double *A;
    double *B;
    double *C;
    int m, n, k;
    int start_row;
    int end_row;
} thread_data_t;

void REF_MMult(int, int, int, double *, int, double *, int, double *, int);
void MY_MMult(int, int, int, double *, int, double *, int, double *, int);
void copy_matrix(int, int, double *, int, double *, int);
void random_matrix(int, int, double *, int);
void zero_matrix(int, int, double *, int);
double compare_matrices(int, int, double *, int, double *, int);
void print_rowmajor_matrix(int m, int n, double *a, int lda);
double dclock();
void MY_MMult_blas(int, int, int, double *, int, double *, int, double *, int);
void MY_MMult_pthread(int m, int n, int k, double *A, int lda, double *B, int ldb, double *C, int ldc);
void MY_MMult_openmp(int m, int n, int k, double *A, int lda, double *B, int ldb, double *C, int ldc);

int main()
{
  int
      p,
      m, n, k,
      lda, ldb, ldc,
      rep;

  double
      dtime,
      dtime_best,
      gflops,
      diff;

  double
      *a,
      *b, *c,
      *cref, // Reference value computed by REF_MMult()
      *cold; // Random Initialization value

  printf("MY_MMult = [\n");

  for (p = PFIRST; p <= PLAST; p = 2 * p)
  {
    m = (M == -1 ? p : M);
    n = (N == -1 ? p : N);
    k = (K == -1 ? p : K);

    gflops = 2.0 * m * n * k * 1.0e-09;

    // ldx for column major
    // lda = (LDA == -1 ? m : LDA);
    // ldb = (LDB == -1 ? k : LDB);
    // ldc = (LDC == -1 ? m : LDC);

    lda = (LDA == -1 ? k : LDA);
    ldb = (LDB == -1 ? n : LDB);
    ldc = (LDC == -1 ? n : LDC);

    /* Allocate space for the matrices */
    /* Note: I create an extra column in A to make sure that
       prefetching beyond the matrix does not cause a segfault */
    a = (double *)malloc(lda * (k + 1) * sizeof(double));
    b = (double *)malloc(ldb * n * sizeof(double));
    c = (double *)malloc(ldc * n * sizeof(double));
    cold = (double *)malloc(ldc * n * sizeof(double));
    cref = (double *)malloc(ldc * n * sizeof(double));

    /* Generate random matrices A, B, Cold */
    random_matrix(m, k, a, lda);
    random_matrix(k, n, b, ldb);
    random_matrix(m, n, cold, ldc);

    copy_matrix(m, n, cold, ldc, cref, ldc);

    /* Run the reference implementation so the answers can be compared */
    //navie矩阵乘REF_MMult函数
    REF_MMult(m, n, k, a, lda, b, ldb, cref, ldc);

    // openblas矩阵乘
    for (rep = 0; rep < NREPEATS; rep++)
    {
      copy_matrix(m, n, cold, ldc, c, ldc);

      /* Time your implementation */
      dtime = dclock();
      MY_MMult_blas(m, n, k, a, lda, b, ldb, c, ldc);
      dtime = dclock() - dtime;

      if (rep == 0)
        dtime_best = dtime;
      else
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
    }


    diff = compare_matrices(m, n, c, ldc, cref, ldc);

    printf("%d %le %le \n", p, gflops / dtime_best, diff);
    fflush(stdout);

    // pthread多线程矩阵乘
    for (rep = 0; rep < NREPEATS; rep++)
    {
      copy_matrix(m, n, cold, ldc, c, ldc);

      /* Time your implementation */
      dtime = dclock();
      MY_MMult_pthread(m, n, k, a, lda, b, ldb, c, ldc);
      dtime = dclock() - dtime;

      if (rep == 0)
        dtime_best = dtime;
      else
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
    }


    diff = compare_matrices(m, n, c, ldc, cref, ldc);

    printf("%d %le %le \n", p, gflops / dtime_best, diff);
    fflush(stdout);

    // openMP矩阵乘
    for (rep = 0; rep < NREPEATS; rep++)
    {
      copy_matrix(m, n, cold, ldc, c, ldc);

      /* Time your implementation */
      dtime = dclock();
      MY_MMult_openmp(m, n, k, a, lda, b, ldb, c, ldc);
      dtime = dclock() - dtime;

      if (rep == 0)
        dtime_best = dtime;
      else
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
    }


    diff = compare_matrices(m, n, c, ldc, cref, ldc);

    printf("%d %le %le \n", p, gflops / dtime_best, diff);
    fflush(stdout);

    free(a);
    free(b);
    free(c);
    free(cold);
    free(cref);
  }

  printf("];\n");

  exit(0);
}

