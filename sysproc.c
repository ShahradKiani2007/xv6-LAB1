#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "fcntl.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_simple_arithmetic_syscall(void)
{
  int a = myproc()->tf->ebx;
  int b = myproc()->tf->ecx;
  int r = (a + b) * (a - b);

  cprintf("Calc: (%d+%d)*(%d-%d) = %d\n", a, b, a, b, r);
  return r;
}

int sys_mprotect(void)
{
  int addr, len, prot;
  struct proc *curproc = myproc();

  if(argint(0, &addr) < 0 || argint(1, &len) < 0 || argint(2, &prot) < 0)
    return -1;
  if(addr < 0 || addr % PGSIZE != 0 || len <= 0)
    return -1;
  if(prot != PROT_READ && prot != (PROT_READ | PROT_WRITE))
    return -1;
  if(addr >= curproc->sz || len > curproc->sz - addr)
    return -1;
  return protectuvm(curproc->pgdir, addr, PGROUNDUP(len), prot & PROT_WRITE);
}
