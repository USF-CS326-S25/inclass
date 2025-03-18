#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int fd;

  fd = fork();

  if(fd == 0){
    printf("In child\n");
    exit(0);
  } else {
    printf("In parent\n");
    wait(0);
  }

  printf("Parent finished.\n");
}
