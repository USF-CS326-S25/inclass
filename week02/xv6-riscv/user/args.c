#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
  int i;

  for(i = 0; i < argc; i++)
    printf("argv[%d] = %s\n", i, argv[i]);
  
  exit(0);
}
