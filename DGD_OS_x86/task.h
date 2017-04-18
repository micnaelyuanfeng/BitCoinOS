#ifndef _TASK_H_
#define _TASK_H_

#include "types.h"
#include "pmm.h"
#include "vmm.h"

typedef enum task_state{
   TASK_UNINT = 0,
   TASK_SLEEP = 1,
   TASK_RUNNABLE_RUNNING = 2,
   TASK_ZOMBIE =3,
} task_state;

struct context{
  uint32_t esp;
  uint32_t ebp;
  uint32_t ebx;
  uint32_t esi;
  uint32_t edi;
  uint32_t eflags;
};

struct mm_struct{
  pgd_t *pgd_dir;  //process table
};

//PCB
struct task_struct{
  volatile task_state state; //process state
  pid_t pid;
  void *stack; //process stack address
  struct mm_struct *mm; //process memory address mapped
  struct context context; //process execute context
  struct task_struct * next;
};

extern pid_t now_pid;

//kernel_thread
int32_t kernel_thread(int (*fn)(void *), void *arg);

void kthread_exit(void);

#endif
