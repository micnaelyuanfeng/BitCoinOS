#ifndef _PMM_H_
#define _PMM_H_

#include "multiboot.h"

#define STACK_SIZE 8192
#define PMM_MAX_SIZE 0x20000000
#define PMM_PAGE_SIZE 0x1000
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)

extern uint8_t kern_start[];
extern uint8_t kern_end[];
//extern uint32_t kern_stack_top;

void pmm_free_page(uint32_t p);
void show_memory_map(void);
void init_pmm(void);

uint32_t pmm_alloc_page();

#endif
