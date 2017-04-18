#include "sched.h"
#include "heap.h"
#include "debug.h"
#include "pmm.h"
#include "console.h"

//runnale process list
struct task_struct *running_proc_head = NULL;
//waiting process list
struct task_struct *wait_proc_head = NULL;
//running process
struct task_struct *current = NULL;

extern uint32_t kern_stack_top;

//static uint32_t counter = 0;

void init_sched(){
  current = (struct task_struct *)(kern_stack_top - STACK_SIZE);

  current->state = TASK_RUNNABLE_RUNNING;
  current->pid = now_pid++;
  current->stack = current;
  current->mm = NULL;

  current->next = current;

  running_proc_head = current;
}

//generic dummy scheduler
void schedule(){
  //printk_color(black, green, "get called\r");
  if(current){
    change_task_to(current->next);
  }
}

void change_task_to(struct task_struct *next){
  if(current != next){
    struct task_struct* prev = current;
    current = next;
    switch_to (&(prev->context), &(current->context));
  }
}
