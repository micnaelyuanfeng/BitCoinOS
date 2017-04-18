#include "pmm.h"
#include "multiboot.h"
#include "debug.h"
#include "comman.h"

static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];
static uint32_t pmm_stack_top = 0;
uint32_t phy_page_count = 0;

void show_memory_map(){
  uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
  uint32_t mmap_length = glb_mboot_ptr->mmap_length;

  printk_color(black, magenta, "Memory Segmentations\n");

  mmap_entry_t *mmap = (mmap_entry_t *) mmap_addr;

  for(; (uint32_t)mmap < mmap_addr + mmap_length; mmap++){
    printk_color(black, cyan, "Base Address = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
                 (uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low,
                 (uint32_t)mmap->length_high, (uint32_t)mmap->length_low,
                 (uint32_t)mmap->type);

    //printk_color(black, cyan, " type = 0x%x\n", (uint32_t)mmap->type);
  }

}

void init_pmm(){
  mmap_entry_t *mmap_start = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
  mmap_entry_t *mmap_end = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;

  mmap_entry_t *map_entry;

  //printk("PAGE_MAX_SIZE: %d\n", PAGE_MAX_SIZE);

  for(map_entry = mmap_start; map_entry < mmap_end; map_entry++){
  //  printk("%d\n", map_entry->base_addr_low);
    if(map_entry->type == 1 && map_entry->base_addr_low == 0x100000){
        uint32_t page_addr = map_entry->base_addr_low + (uint32_t)(kern_end - kern_start);
        uint32_t length = map_entry->base_addr_low + map_entry->length_low;

        while(page_addr < length && page_addr <= PMM_MAX_SIZE){
          pmm_free_page(page_addr);
          //printk_color(black, cyan, "0x%08x", map_entry->base_addr_low);
          page_addr += PMM_PAGE_SIZE;
          phy_page_count ++;
        }
    }
  }
}

uint32_t pmm_alloc_page(){
  assert(pmm_stack_top != 0, "out of memory");
  uint32_t page = pmm_stack[pmm_stack_top--];
  return page;
}

void pmm_free_page(uint32_t p){
  assert(pmm_stack_top != PAGE_MAX_SIZE, "out of pmm_stack");
  pmm_stack[++pmm_stack_top] = p;
}
