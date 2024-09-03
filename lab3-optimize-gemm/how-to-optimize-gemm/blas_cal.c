#include <cblas.h>

// int main(){
//     cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
//     M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
// }

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,    
   m, n, k, 1.0, a, lda, b, ldb, 1.0, c, ldc);
}