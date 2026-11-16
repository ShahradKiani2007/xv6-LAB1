#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  int pid;

  if(fork() == 0){
    if(fork() == 0){
      if(fork() == 0)
        get_ancestors(getpid());
      wait();
      exit();
    }
    wait();
    exit();
  }
  wait();

  get_ancestors(1);
  printf(1, "get_ancestors(9999) = %d\n", get_ancestors(9999));

  pid = fork();
  if(pid == 0)
    exit();
  sleep(10);
  printf(1, "zombie: ");
  get_ancestors(pid);
  wait();

  if(fork() == 0){
    if(fork() == 0){
      sleep(20);
      printf(1, "orphan: ");
      get_ancestors(getpid());
    }
    exit();
  }
  wait();
  sleep(50);
  exit();
}
