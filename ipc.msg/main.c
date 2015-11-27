#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#include <errno.h>

#define KEY 99
#define SRV_CMDLINE "-srv"


int input_available()
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  int rv = select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  if (rv < 0){
      exit(-1);
      perror("select()");
  }
  return (FD_ISSET(0, &fds));
}

#define MAX_TEXT 2048
typedef struct text_message {
  long type;
  char text[MAX_TEXT];
 } text_message;

int client(){
    int msg_id = msgget(KEY, 0666 | IPC_CREAT | IPC_NOWAIT);
    perror("Creating IPC");
    if (msg_id < -1){
        return(-1);
    }

    while(1){
        if (feof(stdin)){
            exit(0);
        }
        if (input_available()){
            text_message msg;
            msg.type = getpid();

            fgets(msg.text, MAX_TEXT, stdin);
            int rv = msgsnd(msg_id, &msg,  MAX_TEXT, IPC_NOWAIT);
            if (rv < 0){
                perror("msgsnd()");
                return(-1);
            }
        }

        text_message msg;
        int rv = msgrcv(msg_id, &msg, MAX_TEXT, getpid(), IPC_NOWAIT | MSG_EXCEPT);
        if (rv < 0 && errno != ENOMSG){
            perror("msgrcv()");
            return(-1);
        } else if (rv > 0){
            printf("rcv %d > %s\n", msg.type, msg.text);
        }
        usleep(100);
    }

    return 0;
}

int main(int argc, char **argv){
    printf("Running client mode\n");
    exit(client());

}
