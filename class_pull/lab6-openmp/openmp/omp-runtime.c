#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
int main(int argc, char *argv[]) {

#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    printf("Hello World from thread %d of %d\n",
    thread_id, num_threads);
}
    return(0);
}
