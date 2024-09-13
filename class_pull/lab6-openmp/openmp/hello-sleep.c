#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main(void)
{
    #pragma omp parallel
    {
        printf("Hello, world.\n");
        sleep(60);
    }
    return 0;
}
