#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/list.h"
#include "user/user.h"

#define STR_LEN 128

struct line_node {
  struct list_elem elem;
  char line_text[STR_LEN];
};

void
readlines(struct list *lp, int fd)
{
  struct line_node *lnp;
  int rv;
  
  do {
    lnp = (struct line_node *) malloc(sizeof(struct line_node));
    rv = readline(fd, &(lnp->line_text[0]), STR_LEN - 1);
    if (rv > 0)
      list_push_back(lp, &lnp->elem);
    else{
      free(lnp);      
      break;
    }
  } while (1);
}

void
printlines(struct list *lp)
{
  struct list_elem *e;

  for (e = list_begin(lp); e != list_end(lp); e = list_next(e)) {
    struct line_node *lnp = list_entry(e, struct line_node, elem);
    printf("%s", lnp->line_text);
  }
  return;
}

int
main(int argc, char *argv[])
{
  struct list lines;
  int fd;

  if(argc != 2){
    printf("usage: echofile <filename>\n");
    exit(-1);
  }

  fd = open(argv[1], O_RDONLY);
  if(fd < 0){
    printf("echofile: cannot open %s\n", argv[1]);
    exit(-1);
  }

  list_init(&lines);
  readlines(&lines, fd);
  printlines(&lines);
}
