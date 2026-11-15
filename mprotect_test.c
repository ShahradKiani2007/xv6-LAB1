#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  char *p = sbrk(4096);

  p[0] = 'A';
  printf(1, "write before protect: ok\n");
  mprotect(p, 4096, PROT_READ);
  printf(1, "read after protect: %c\n", p[0]);

  if(fork() == 0){
    p[0] = 'B';
    printf(1, "NOT KILLED\n");
    exit();
  }
  wait();
  printf(1, "child was killed by the kernel\n");

  mprotect(p, 4096, PROT_READ | PROT_WRITE);
  p[0] = 'C';
  printf(1, "write after PROT_WRITE: ok\n");

  printf(1, "unaligned: %d\n", mprotect(p + 1, 4096, PROT_READ));
  printf(1, "len 0: %d\n", mprotect(p, 0, PROT_READ));
  printf(1, "kernel address: %d\n", mprotect((void*)0x80000000, 4096, PROT_READ));
  printf(1, "outside process: %d\n", mprotect(p + 4096, 4096, PROT_READ));
  printf(1, "write only: %d\n", mprotect(p, 4096, PROT_WRITE));

  printf(1, "two pages: %d\n", mprotect(p, 8192, PROT_READ));
  p[0] = 'D';
  printf(1, "first page still writable: ok\n");
  exit();
}
