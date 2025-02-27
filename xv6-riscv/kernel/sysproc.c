#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_spoon(void)
{
  // obtain the argument from the stack, we need some special handling
  uint64 addr;
  argaddr(0, &addr);
  return spoon((void*)addr);
}

uint64
sys_clone(void)
{
  uint64 func;
  uint64 arg;
  uint64 stack;

  argaddr(0, &func);
  argaddr(1, &arg);
  argaddr(2, &stack);

  return clone((void(*)(void*)) func, (void*) arg, (void*) stack);
}

uint64
sys_join(void)
{
  uint64 pid;
  uint64 stack;

  argaddr(0, &pid);
  argaddr(1, &stack);

  return join((int) pid, (void**) stack);
}

uint64
sys_texit(void)
{
  return texit();
}
