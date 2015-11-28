#define _GNU_SOURCE

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#include <errno.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>

#define SIZE 100
#define STRING_A "haaa"
#define STRING_B "hooooo"

int main(){
    key_t key = ftok(get_current_dir_name(), 4);
    if (key < 0 ){
        perror("fkey()");
        exit(-1);
    }

    int shm_id = shmget(key, SIZE, IPC_CREAT);
    if (shm_id < 0){
        perror("shmget()");
        exit(-1);
    }

    int sem_id = semget(key, 2, IPC_CREAT|IPC_EXCL);
    if (sem_id < 0 && errno != EEXIST){
        perror("semget()");
        exit(-1);
    }

    if (errno != EEXIST){
        int rv = semctl(sem_id, 0, SETVAL, 1);
        if (rv < 0){
            perror("semctl()");
            exit(-1);
        }
    } else {
        sem_id = semget(key, 2, IPC_CREAT); // we need valid semaphore
        if (sem_id < 0){
            perror("semget2()");
            exit(-1);
        }
    }

    struct sembuf sem = {
        SEM_UNDO | IPC_NOWAIT,
        0,
        -1
    };

    int rv = semop(sem_id, &sem, 1);
    if (rv < 0){
        printf("%d\n", sem_id);
        perror("semop()");
        exit(-1);
    }

    char *p;

    if (errno == EEXIST){
        p = shmat(shm_id, NULL, SHM_RDONLY);
        if (p<0) {
            perror("shmat(,,SHM_RDONLY)");
            exit(-1);
        }
        printf("Reader\n");
        while(1){
            if (strncmp(p, STRING_A, sizeof(STRING_A)) != 0 && strncmp(p, STRING_B, sizeof(STRING_B))!= 0){
                printf("erreur\n");
                break;
            }
        }
    } else {
        printf("Writer\n");
        p = shmat(shm_id, NULL, 0);
        if (p<0) {
            perror("shmat()");
            exit(-1);
        }
        fputs("dupa", stdout);

        while(1){
            strncpy(p, STRING_A, sizeof(STRING_A));
            strncpy(p, STRING_B, sizeof(STRING_B));
        }
    }

	return 0;

}
