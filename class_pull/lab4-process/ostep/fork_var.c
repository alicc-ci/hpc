#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g_x;
int 
main(int argc, char *argv[])
{
    int main_x=0;
    printf("hello world (pid:%d)\n\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        printf("child process(%d) first access:g_x=%d, main_x=%d\n", 
                    (int) getpid(), g_x, main_x);
        g_x = 2;
        main_x = 2;
        printf("child process(%d) second access:g_x=%d, main_x=%d\n",
                     (int) getpid(), g_x, main_x);
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
	       rc, (int) getpid());
        printf("parent process(%d) first access:g_x=%d, main_x=%d\n", 
                    (int) getpid(), g_x, main_x);
    
        g_x = 3;
        main_x = 3;
        printf("parent process(%d) second access:g_x=%d, main_x=%d\n", 
                    (int) getpid(), g_x, main_x);
    }
    printf("process(%d) last access:g_x=%d, main_x=%d\n", 
                (int) getpid(), g_x, main_x);
    printf("\n");
    return 0;

}
