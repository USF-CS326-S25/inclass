#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/list.h"
#include "user/user.h"

#define STR_LEN 128

struct pair {
  struct list_elem elem;
  char name[STR_LEN];
  char value[STR_LEN];
};

void
pair_list_print(struct list *lp)
{
  struct list_elem *e;

  for (e = list_begin(lp); e != list_end(lp); e = list_next(e)) {
    struct pair *p = list_entry(e, struct pair, elem);
    printf("[%s, %s]\n", p->name, p->value);
  }
  return;
}

bool
pair_list_less(const struct list_elem *a, const struct list_elem *b, void *aux)
{
  struct pair *ap;
  struct pair *bp;

  ap = list_entry(a, struct pair, elem);
  bp = list_entry(b, struct pair, elem);  
  
  return (strcmp(ap->name, bp->name) < 0);
}

int
main(int argc, char *argv[])
{
  struct list dmap;
  struct pair p1;
  struct pair p2;
  struct pair p3;
  struct pair p4;

  list_init(&dmap);

  strcpy(p1.name, "google.com");
  strcpy(p1.value, "142.251.46.174");
  list_push_back(&dmap, &p1.elem);

  strcpy(p2.name, "usfca.edu");
  strcpy(p2.value, "23.185.0.2");
  list_push_back(&dmap, &p2.elem);

  strcpy(p3.name, "mit.edu");
  strcpy(p3.value, "104.90.21.210");
  list_push_back(&dmap, &p3.elem);

  strcpy(p4.name, "openai.com");
  strcpy(p4.value, "13.107.238.57");
  list_push_back(&dmap, &p4.elem);

  pair_list_print(&dmap);

  /* Sort list */
  list_sort(&dmap, pair_list_less, (void *) 0);
  printf("Sorted:\n");
  pair_list_print(&dmap);
  
  exit(0);
}
