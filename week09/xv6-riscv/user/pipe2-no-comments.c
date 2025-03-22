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

  pipe(pfd);

  pid = fork();

  if(pid == 0){
    printf("In child\n");
    close(pfd[0]);
    write(pfd[1], "foo\n", 4);
    sleep(50);
    write(pfd[1], "bar\n", 4);
    write(pfd[1], "baz\n", 4);
    close(pfd[1]);
    exit(0);
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
