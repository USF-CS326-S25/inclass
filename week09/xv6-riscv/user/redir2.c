#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define NULL 0

int
main(int argc, char *argv[])
{
  int pid, fd;
  char *argv_child[10];
  argv_child[0] = "ls";
  argv_child[1] = NULL;
  int rv;

  pid = fork();

  if(pid == 0){
    printf("In child\n");

    close(1); // close stdout

    fd = open("out.txt", O_CREATE | O_WRONLY);
    if(fd < 0){
      printf("Cannot open(\"%s\")\n", "out.txt");
      exit(-1);
    }

    rv = exec(argv_child[0], argv_child);
    if(rv < 0)
      printf("Cannot exec(\"%s\")\n", argv_child[0]);
    exit(0);
  } else {
    printf("In parent\n");
    wait(0);
  }

  printf("Parent finished.\n");
}
