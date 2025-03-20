#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define NULL 0

int
main(int argc, char *argv[])
{
  int pid;
  int rv;
  int pfd[2];
  char buf[16];
  char *argv_child[10];

  pipe(pfd);

  pid = fork();

  if(pid == 0){
    printf("In child\n");
    argv_child[0] = "ls";
    argv_child[1] = NULL;

    close(1);
    dup(pfd[1]);
    close(pfd[1]);
    close(pfd[0]);

    rv = exec(argv_child[0], argv_child);
    if(rv < 0){
      printf("Cannot exec(\"%s\")\n", argv_child[0]);
      exit(-1);
    }
  } else {
    printf("In parent\n");
    close(pfd[1]);
    while(1){
      rv = read(pfd[0], buf, 1);
      if(rv > 0){
        buf[1] = '\0';
        printf("%s", buf);
      } else
        break;
    }
    close(pfd[0]);
    wait(0);
  }

  printf("Parent finished.\n");
}
