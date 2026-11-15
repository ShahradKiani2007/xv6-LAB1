#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  printf(1, "%d\n", simple_arithmetic_syscall(5, 3));
  printf(1, "%d\n", simple_arithmetic_syscall(0, 4));
  printf(1, "%d\n", simple_arithmetic_syscall(-2, 5));
  exit();
}
