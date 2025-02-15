#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/list.h"
#include "user/user.h"

// We will use the globals from umalloc.c
extern struct list block_list;
extern uint heap_start;
extern uint heap_initialized;
extern uint heap_start;

// The heap allocator block_hdr struct from umalloc.c
struct block_hdr {
  struct list_elem elem;
  char name[8];
  uint used;
  uint size;
};

int
main(int argc, char **argv)
{
  void *p;
  struct block_hdr *hp;
  struct block_hdr *fp;
  uint size;
  uint req;

  // Let malloc_print() know that the heap is initialized
  heap_initialized = 1;

  // Initialize the block list
  list_init(&block_list);

  // Allocate heap space from the kernel
  req = 4096;
  p = sbrk(req);
  heap_start = (long) p;
 
  // Allocate a block of memory, 40 bytes
  size = 40;
  hp = (struct block_hdr *) p;
  hp->used = 1;
  hp->size = size;
  strcpy(hp->name, "block1");
  // Put the block on the block list
  list_push_back(&block_list, &(hp->elem));

  // p points to the user data in the block, but we are not using it here
  p = ((void *) hp) + sizeof(struct block_hdr);

  // The free block will start after the used block
  fp = (struct block_hdr *) (((void *) hp) 
       + sizeof(struct block_hdr) + size);

  fp->used = 0;

  // The size of the free block is the remaining space
  // after the used block header + used block size
  //  and the free block header
  fp->size = req - (sizeof(struct block_hdr) + size
                    + sizeof(struct block_hdr));

  // Put the free block on the block list
  list_push_back(&block_list, &(fp->elem));

  malloc_print();

  exit(0);
}
