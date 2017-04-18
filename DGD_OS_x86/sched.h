#ifndef _SCHED_H_
#define _SCHED_H_

#include "task.h"

//process waiting list head
extern struct task_struct *running_proc_head;

//process waiting list head
extern struct task_struct *wait_proc_head;

//current running process
extern struct task_struct *current;

//initialize process list
void init_sched(void);

void schedule(void);

void change_task_to(struct task_struct *next);

void switch_to(struct context* prev, struct context* next);

#endif
