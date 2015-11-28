#define _GNU_SOURCE

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#include <errno.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE 100
#define STRING_A "haaa"
#define STRING_B "hooooo"

int main(int argc, char **argv){
    key_t key = ftok(get_current_dir_name(), 8);
    if (key < 0 ){
        perror("fkey()");
        exit(-1);
    }

    int shm_id = shmget(key, SIZE, 0666 | IPC_CREAT);
    if (shm_id < 0){
        perror("shmget()");
        exit(-1);
    }
    assert(argc > 1);


    char *p = shmat(shm_id, NULL, 0);
    printf("XXXy %d\n", p);

    if (p < 0) {
        perror("shmat()");
        exit(-1);
    }
    if (strncmp(argv[1], "w", sizeof("w"))!=0){

        printf("Reader\n");
        while(1){
            if (strncmp(p, STRING_A, sizeof(STRING_A)) != 0 && strncmp(p, STRING_B, sizeof(STRING_B))!= 0){
                printf("erreur %s\n", p);
                printf("%d %d\n", strncmp(p, STRING_A, sizeof(STRING_A)), strncmp(p, STRING_B, sizeof(STRING_B)));
                break;
            }
        }
    } else {
        printf("Writer\n");

        while(1){
            strncpy(p, STRING_A, sizeof(STRING_A));
            strncpy(p, STRING_B, sizeof(STRING_B));
        }
    }

	return 0;

}
