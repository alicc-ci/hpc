#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include "defs.h"
#include <sys/time.h>
#include <pthread.h>
#include "MY_MMult.c"

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
    REF_MMult(m, n, k, a, lda, b, ldb, cref, ldc);

    /* Time the "optimized" implementation */
    for (rep = 0; rep < NREPEATS; rep++)
    {
      copy_matrix(m, n, cold, ldc, c, ldc);

      /* Time your implementation */
      dtime = dclock();
      MY_MMult(m, n, k, a, lda, b, ldb, c, ldc);
      dtime = dclock() - dtime;

      if (rep == 0)
        dtime_best = dtime;
      else
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
    }

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

    free(a);
    free(b);
    free(c);
    free(cold);
    free(cref);
  }

  printf("];\n");

  exit(0);
}

// 多线程DGEMM计算部分
void *thread_dgemm(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    double *A = data->A;
    double *B = data->B;
    double *C = data->C;
    int m = data->m;
    int n = data->n;
    int k = data->k;
    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int p = 0; p < k; p++) {
                sum += A[i * k + p] * B[p * n + j];
            }
            C[i * n + j] = sum;
        }
    }

    pthread_exit(NULL);
}

void MY_MMult_pthread(int m, int n, int k, double *A, int lda, double *B, int ldb, double *C, int ldc) {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int rows_per_thread = m / NUM_THREADS;

    // 记录起始时间
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // 分配每个线程负责的行
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        thread_data[i].m = m;
        thread_data[i].n = n;
        thread_data[i].k = k;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == NUM_THREADS - 1) ? m : (i + 1) * rows_per_thread;

        pthread_create(&threads[i], NULL, thread_dgemm, (void *)&thread_data[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 记录结束时间
    gettimeofday(&end, NULL);

    // 计算时间差
    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    // printf("MY_MMult (多线程) 完成，耗时: %f 秒\n", time_taken);
}