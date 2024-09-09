#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N 2048  // 矩阵大小

// 初始化矩阵
void init_matrix(double **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (double)(rand() % 100);
        }
    }
}

// Naive 矩阵乘法实现 C = A * B + C
void naive_gemm(double **A, double **B, double **C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    double **A, **B, **C;
    pid_t pid;

    // 动态分配内存
    A = (double **)malloc(N * sizeof(double *));
    B = (double **)malloc(N * sizeof(double *));
    C = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
        B[i] = (double *)malloc(N * sizeof(double));
        C[i] = (double *)malloc(N * sizeof(double));
    }

    // 初始化矩阵 A, B, C
    init_matrix(A);
    init_matrix(B);
    init_matrix(C);

    // 调用 fork
    pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }

    if (pid == 0) {
        // 子进程
        printf("Child process started.\n");
        naive_gemm(A, B, C);
        printf("Child process completed.\n");
    } else {
        // 父进程
        printf("Parent process started.\n");
        naive_gemm(A, B, C);
        wait(NULL);  // 等待子进程完成
        printf("Parent process completed.\n");
    }

    // 释放动态分配的内存
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
