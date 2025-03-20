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
    // Child 1 (C1)

    printf("In child 1\n");
    close(pfd[0]);
    write(pfd[1], "foo\n", 4);
    write(pfd[1], "bar\n", 4);
    write(pfd[1], "baz\n", 4);
    close(pfd[1]);
    exit(0);
  }

  // In the parent

  pid = fork();

  if(pid == 0){
    // Child 2 (C2)
    printf("In child 2\n");
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
    exit(0);
  }

  // In the parent
  close(pfd[0]);
  close(pfd[1]);

  wait(0);
  wait(0);

  printf("Parent finished.\n");
}
