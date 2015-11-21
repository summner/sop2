#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#define MAXS 64

int main(int argc, char **argv){
    printf("--------poczatek listy------------- \n");

    {
      DIR           *d;
      struct dirent *dir;
      d = opendir("/proc");
      char tmp[1000];
      if (d)
      {
        while ((dir = readdir(d)) != NULL)
        {
          int pid = atoi(dir->d_name);
          if (pid > 0){
              snprintf(tmp, sizeof(tmp),"/proc/%d/comm", pid);
              int fd = open(tmp, O_RDONLY);
              memset(tmp, 0, sizeof(tmp));
              int r = read(fd, tmp, sizeof(tmp));

              printf("%s", tmp);
              close(fd);
//            printf("%d\n", pid);
          }
        }

        closedir(d);
      }

    }
    printf("--------koniec listy-------------\n");
	return 0;
}
