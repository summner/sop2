#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>

#define MAXS 64

int blah(int fd)
{
    int pid;
    pid = fork();
    char tmp[1000];
    if (pid == 0){
        while(1){
            int r = read(fd, tmp, 999);
            tmp[r] = '\0';
            printf("\t%d read: %s\n", getpid(),tmp);
        }
    }
    return pid;
}

int main(int argc, char **argv){
    printf("start\n");
    fflush(stdout);
    char *mfifo = "/tmp/dupdupdup";

    mkfifo(mfifo, 0666);

    int fd = open(mfifo, O_RDWR);

    int pid = fork();
    if (pid == 0){

        while(1){
            int w = write(fd, "Hi", sizeof("Hi"));
            usleep(500000);
        }
    } else {
        if (blah(fd)!= 0) if (blah(fd)!=0) blah(fd);
    }
    while(1){
        int dup;
        wait(&dup);
    }
	return 0;
}
