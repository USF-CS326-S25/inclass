#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/getkd.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct kd_st kd;
  int rv;

  printf("&kd = %p\n", &kd);
  rv = getkd(&kd);
  printf("getkd() = %d\n", rv);

  printf("kdp->count_used = %d\n", kd.count_used);
  printf("kdp->count_free = %d\n", kd.count_free);
}
