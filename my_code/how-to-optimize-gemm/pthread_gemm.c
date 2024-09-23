#include <stdio.h>
#include <omp.h>
#include "defs.h"
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 4  // 定义线程数量

typedef struct {
    double *A;
    double *B;
    double *C;
    int m, n, k;
    int start_row;
    int end_row;
} thread_data_t;

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