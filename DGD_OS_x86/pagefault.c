#include "debug.h"
#include "vmm.h"

void page_fault(pt_regs *regs){
  uint32_t cr2;
  asm volatile ("mov %%cr2, %0" : "=r"(cr2));

  printk_color(black, green, "Page fault @ 0x%x, vritual faulting address 0x%0x\n", regs->eip, cr2);
  printk_color(black, green, "Error Code %x\n", regs->err_code);
}
