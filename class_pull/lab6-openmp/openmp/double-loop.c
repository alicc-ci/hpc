#include<stdio.h>
#include<unistd.h>
#include<omp.h>

int main(){
    #pragma omp parallel for
    for(int i=0; i<2; i++){
        for(int j=0; j<3; j++){
            printf("i=%d j=%d\n from thread =%d \n", 
                    i, j, omp_get_thread_num());
        }
    }
}