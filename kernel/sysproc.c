#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
//
#include "pstat.h"
#include "proc.h"
int read_calls = -1;

//extern const int total_tickets;
struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} plock;

int counter = 0;
int
sys_settickets(int number){
  if(argint(0, &number) < 0 || number < 1)
  {
    return -1;
    
  }
  else
  {
    acquire(&plock.lock);
    myproc()->tickets = number;
    release(&plock.lock);
  }
  return 0;
}

int 
sys_getpinfo(struct pstat* procStat)
{
  struct proc *p;
  int i = 0;
  if(argint(0, (int*)(&procStat))<0) return -1;
  acquire(&plock.lock);
  for(p = plock.proc; p < &plock.proc[NPROC]; p++)
  {
    procStat->inuse[i] = p->inuse;
    procStat->tickets[i] = p->tickets;
    procStat->pid[i] = p->pid;
    procStat->ticks[i] = p->ticks;
    i++;
  }
  release(&plock.lock);
  return 0;
}
/*
int 
sys_getpinfo(struct pstat* target){
  acquire(&public_ptable.lock);
  if(argint(0,(int*)(&target)) < 0)
  {
    return -1;
  }
  for (struct proc* p = ptable-proc;p != &(ptable.proc[NPROC]);p++)
  {
    const int index = p - ptable.proc;
    if(p->state != UNUSED)
    {
      target->pid[index] = p->pid;
      target->ticks[index] = p->ticks;
      target->tickets[index] = p->tickets;
      //target
    }
  }
}
*/
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

int
sys_getreadcount(void){
  if (read_calls == -1) return read_calls;
  else return read_calls+1;
}
