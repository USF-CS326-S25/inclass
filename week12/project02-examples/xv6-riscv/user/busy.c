#include "kernel/types.h"
#include "user/user.h"

void
print_busy(char *name, char *state)
{
  printf("[%s, %d, %d] busy - %s\n", 
         name, getpid(), uptime(), state);
}

int
main(int argc, char **argv)
{
  char *name;
  long ticks;
  long ticks_start;
  
  if(argc != 3){
    fprintf(2, "usage: busy <name> <ticks>\n");
    exit(-1);
  }

  name = argv[1];
  ticks = atoi(argv[2]);

  print_busy(name, "start");

  ticks_start = uptime();

  while((uptime()-ticks_start) < ticks);

  print_busy(name, "end");

  return 0;
}
