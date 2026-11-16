#include "types.h"
#include "stat.h"
#include "user.h"

int child(int ticks)
{
  int pid = fork();

  if(pid == 0){
    sleep(ticks);
    exit();
  }
  return pid;
}

int main(void)
{
  int a = child(30);
  int b = child(10);
  int c = child(20);

  printf(1, "waitpid(%d) = %d\n", a, waitpid(a));
  printf(1, "waitpid(%d) = %d\n", b, waitpid(b));
  printf(1, "waitpid(%d) = %d\n", c, waitpid(c));

  child(5);
  printf(1, "waitpid(-1) = %d\n", waitpid(-1));
  printf(1, "waitpid(getpid()) = %d\n", waitpid(getpid()));
  printf(1, "waitpid(%d) again = %d\n", a, waitpid(a));
  printf(1, "waitpid(0) = %d\n", waitpid(0));

  child(5);
  printf(1, "wait() = %d\n", wait());
  exit();
}
