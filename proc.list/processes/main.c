#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#define MAXS 64

int main(int argc, char **argv){
    printf("start\n");
    fflush(stdout);
    int pid = fork();
    if (pid == 0){
        printf("child\n");
        if (fork() == 0) exit(0);
        if (fork() == 0) exit(0);
        if (fork() == 0) exit(0);
    } else {
        printf("parent\n");
        exit(0);
    }
    sleep(15);
	return 0;
}
