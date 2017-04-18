#include "idt.h"
#include "vmm.h"
#include "pmm.h"
#include "string.h"
#include "debug.h"

pgd_t pgd_kern[PGD_SIZE] __attribute__((aligned(PAGE_SIZE)));
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_vmm(){
  uint32_t kern_pte_first_idx = PGD_INDEX(PAGE_OFFSET);

  //each table has 128 entries and 1024 2nd page tables
  uint32_t i,j;
  //need to - PAGE_OFFSET to locate right .data section (all virtual address)
  /*Debug
  printk("0x%x\n", pgd_kern);
  printk("0x%x\n", PGD_SIZE);
  printk("0x%x\n", kern_pte_first_idx);
  printk("0x%x\n", &pgd_kern[0]);
  printk("0x%x\n", &pgd_kern[1]);
  printk("0x%x\n", &pgd_kern[2]);
  printk("0x%x\n", &pgd_kern[1023]);
  printk("0x%x\n", &pgd_kern[kern_pte_first_idx]);
  printk("0x%x\n", &pgd_kern[kern_pte_first_idx + 128]);
  */

  for(i = kern_pte_first_idx, j = 0; i < kern_pte_first_idx + PTE_COUNT; i++, j++){
    pgd_kern[i] = ((uint32_t)pte_kern[j] - PAGE_OFFSET)|PAGE_PRESENT|PAGE_WRITE;
  }

  uint32_t *pte = (uint32_t *)pte_kern;
  //page 0 - 512MB
  for(i = 0; i < PTE_COUNT * PTE_SIZE; i++){
    pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
  }

  uint32_t pgd_kern_phy_addr = (uint32_t)pgd_kern - PAGE_OFFSET;

  register_interrupt_handler(14, &page_fault);

  switch_pgd(pgd_kern_phy_addr);
}

void switch_pgd(uint32_t pd){
  asm volatile("mov %0, %%cr3" : : "r"(pd));
}

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags){
  uint32_t pgd_idx = PGD_INDEX(va);
  uint32_t pte_idx = PTE_INDEX(va);

  pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
  //if no mapping
  if(!pte){
      //return first 20 bits w/o last 12 bits
      pte = (pte_t *)pmm_alloc_page();
      pgd_now[pgd_idx] = (uint32_t)pte|PAGE_PRESENT|PAGE_WRITE;

      //reverse back to virtual address
      pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

      bzero(pte, PAGE_SIZE);
  }
  else{
      pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
  }

  pte[pte_idx] = (pa & PAGE_MASK) | flags;

  asm volatile ("invlpg (%0)" : : "a"(va));
}

void unmap(pgd_t *pgd_now, uint32_t va){
  uint32_t pgd_idx = PGD_INDEX(va);
  uint32_t pte_idx = PTE_INDEX(va);

  //get Physical page address
  pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

  //no mapping -> no need unmap and return
  if(!pte)
    return;

  pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

  pte[pte_idx] = 0;

  asm volatile ("invlpg (%0)" : : "a"(va));
}

uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa){
  uint32_t pgd_idx = PGD_INDEX(va);
  uint32_t pte_idx = PTE_INDEX(va);

  //get Physical page address
  pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

  if(!pte){
    return 0;
  }

  //get PTE virtual address to access
  pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

  //get physcal address
  if(pte[pte_idx] != 0 && pa){
    *pa = pte[pte_idx] & PAGE_MASK;
    return 1;
  }

  return 0;

}
