#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
  char *filename;
  int fd;
  char buf[1024];
  int n;
  int len = 0;

  if(argc == 1){
    fd = 0; // stdin
  }else{
    filename = argv[1];
    fd = open(filename, O_RDONLY);
    if(fd<0){
      printf("fd = %d : can't open file %s\n", fd, filename);
      exit(-1);
    }
  }
  
  printf("fd = %d\n", fd);

  while(1){
    n = read(fd, &buf[len], 16);
    if(n <= 0)
      break;
    len += n;
  }

  buf[len] = '\0';
  
  printf("buf = %s\n", buf);
  
  exit(0);
}
