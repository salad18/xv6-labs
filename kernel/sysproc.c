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

void backtrace()
{
  printf("backtrace:\n");
  uint64 fp = r_fp();
  uint64 page = PGROUNDDOWN(fp) + PGSIZE;
  for (; fp < page;)
  {
    printf("%p\n", *((uint64 *)(fp - 8)));
    fp = *((uint64 *)(fp - 16));
  }
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  backtrace();
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


uint64
sys_sigalarm(void)
{
  argint(0, &myproc()->alarm.intervel);
  argaddr(1, &myproc()->alarm.handler);
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  // finish handle
  p->alarm.unfinish = 0;
  // printf("\n sigreturn pc pc:%p\n", p->trapframe->epc);
  memmove(p->trapframe, p->usertrap, sizeof(*p->trapframe));
  // printf("\n restore pc pc:%p\n", p->usertrap->epc);
  return p->trapframe->a0;
}