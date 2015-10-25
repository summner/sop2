#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

int suma(int a, int *b){
    a++;
    (*b)++;
    int c = a + *b;
    printf("%d + %d = %d\n", a, *b, c);
    return c;
}

void a1(int argc, char **argv){
    printf("Nargs: %d\n", argc);
    for(int i=0; i<argc; i++){
        printf("%d: %s\n", argc, argv[i]);
    }

    if (argc >2){
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        int c = suma(a, &b);
        printf("%d + %d = %d\n", a,b ,c);
    }

}


#define MAXS 64
/*
int main(int argc, char **argv){
    char buf[MAXS];

    assert(argc > 2);
    char *src_path = argv[1];
    char *dst_path = argv[2];
    assert(access(src_path, F_OK) != -1);
    int n;
    int src_fd = open("./dup.txt", O_RDONLY);
    int dst_fd = open(dst_path, O_CREAT | O_WRONLY,  S_IRUSR | S_IWUSR);

    int linecnt=0;
    while ((n=read(src_fd, buf, MAXS))){
        for(int i=0; i<n; i++){
            if (buf[i] == '\n'){
                linecnt++;
            }
        }
        write(dst_fd, buf, n);
    }
    int num;
//    int num = lseek(src_fd, 0, SEEK_SET);
    num = lseek(src_fd, 0 , SEEK_END);

    printf("written %d bytes %d lines to file %s\n", num, linecnt, dst_path);
    return 0;
}*/

int main(int argc, char **argv){
    char buf[MAXS];

    unsigned int counts[255] = {0};
    unsigned int total = 0;

    assert(argc > 1);
    char *src_path = argv[1];

    assert(access(src_path, F_OK) != -1);
    int n;
    int src_fd = open(src_path, O_RDONLY);


    while ((n=read(src_fd, buf, MAXS))){

        for(int i=0; i<n; i++){
            char c= buf[i];
            if ((c >= 'A' && c<='Z') || (c>='a' && c<='z')){
                total++;
                counts[buf[i]]++;
            }
        }
    }

    for (char c='A'; c<='Z'; c++){
        if (counts[c] > 0){
            printf("%c: %d%%\n", c, counts[c]*100/total);
        }
    }

    printf("total: %d\n", total);
    for (char c='a'; c<='z'; c++){
        if (counts[c] > 0){
            printf("%c: %d%%\n", c, counts[c]*100/total);
        }
    }

//    printf("written %d bytes %d lines to file %s\n", num, linecnt, dst_path);
    return 0;
}
