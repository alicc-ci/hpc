#include<stdio.h>
#include<unistd.h>
#include<omp.h>

int main(){
    int i=1;
    int j=1;
    #pragma omp parallel for
    for(i=0; i<2; i++){
        for(j=0; j<3; j++){
            printf("i=%d j=%d\n from thread =%d \n", 
                    i, j, omp_get_thread_num());
        }
    }
}