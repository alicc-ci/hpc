#include<stdio.h>
#include<unistd.h>

int main(){
    int N=4;
    int a[N], b[N], c[N];
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        a[i]=i;
        b[i]=i*i;
        c[i]=a[i]+b[i];
        printf("c[%d]=%d\n",i, c[i]);
    }
}