#ifndef _VMM_H_
#define _VMM_H_

#include "types.h"
#include "idt.h"
#include "pmm.h"

#define PAGE_OFFSET 0xC0000000
#define PAGE_PRESENT 0x1 //indicate if this translation is valid
#define PAGE_WRITE 0x2 // 1= read, write, and exe, 0= read, exe, 2= none
#define PAGE_SIZE 4096 //bytes
#define PAGE_MASK 0xFFFFF000  //4K align
#define PGD_INDEX(x) (((x)>>22) & 0x3FF) // get page catalog 1st level entry
#define PTE_INDEX(x) (((x)>>12) & 0x3FF) // get page table entry 2nd level entry
#define OFFSET_INDEX(x) ((x) && 0xFFF) // offset

typedef uint32_t pgd_t;
typedef uint32_t pte_t;

#define PGD_SIZE (PAGE_SIZE/sizeof(pte_t)) //2^10, 32 bits address
#define PTE_SIZE (PAGE_SIZE/sizeof(uint32_t)) //2^10, 32 bits address
#define PTE_COUNT 128 //# of PGT entries = 512MB ram /(4KB page * 1024 entries per pte)

extern pgd_t pgd_kern[PGD_SIZE]; //1024 page table index array

void init_vmm(void);

//change page table catalog
void switch_pgd(uint32_t pd);

//check previllege and translation
void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);

void unmap(pgd_t *pgd_now, uint32_t va);

uint32_t get_mapping(pgd_t *pgt_now, uint32_t va, uint32_t *pa);

//interrupt when page fault
extern void page_fault(pt_regs *regs);

#endif
