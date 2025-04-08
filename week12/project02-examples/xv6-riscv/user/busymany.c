#include "kernel/types.h"
#include "user/user.h"

void
print_busy(char *name, char *state)
{
  printf("[%s, %d, %d] busy - %s\n",
    name, getpid(), uptime(), state);
}

void
busy(char *name, long ticks)
{
  long ticks_start;

  //print_busy(name, "start");

  ticks_start = uptime();

  if(ticks < 10)
    while((uptime()-ticks_start) < ticks);
  else
    while((uptime()-ticks_start) < ticks)
      sleep(5);

  //print_busy(name, "end");
  return;
}

int
main(int argc, char **argv)
{
  long ticks;
  int count = 0;
  int id;
  char name[16];

  if(argc != 3){
    fprintf(2, "usage: busy <ticks> <count>\n");
    exit(-1);
  }

  ticks = atoi(argv[1]);
  count = atoi(argv[2]);

  for(int i = 0; i < count; i++){
    id = fork();
    if(id == 0){
      strcpy(name, "busy[");
      name[5] = 'A' + i;
      name[6] = ']';
      name[7] = '\0';
      sname(name);
      busy(name, ticks);
      exit(0);
    }
  }

  return 0;
}
