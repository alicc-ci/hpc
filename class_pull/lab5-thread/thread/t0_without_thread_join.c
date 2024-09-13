#include <stdio.h> 
#include <assert.h> 
#include <pthread.h> 
#include <unistd.h>

void *mythread(void *arg) { 
    printf("thread start %s\n", (char *)arg); 
    sleep(5);
    pthread_t tid = pthread_self();
    printf("pid:%d, tid:%u, %s\n",  (int) getpid(), (unsigned int)tid, (char *) arg); 
    return NULL; 
} 

int main(int argc, char *argv[]) { 
    pthread_t p0, p1, p2; 
    int rc; 

    p0 = pthread_self();
    printf("main begin, tid p0:%u\n", (unsigned int)p0); 
    rc = pthread_create(&p1, NULL, mythread, "A"); assert(rc == 0); 
    rc = pthread_create(&p2, NULL, mythread, "B"); assert(rc == 0); 
    // join waits for the threads to finish 
    printf("pthread created, tid p1:%u, p2:%u\n", (unsigned int)p1, (unsigned int)p2); 
    // rc = pthread_join(p1, NULL); assert(rc == 0); 
    // rc = pthread_join(p2, NULL); assert(rc == 0); 
    printf("main end\n"); 
    return 0; 
}