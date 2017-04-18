#include "debug.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"

static void alloc_chunk(uint32_t start, uint32_t len);

static void free_chunk(header_t *chunk);

static void split_chunk(header_t *chunk, uint32_t len);

static void glue_chunk(header_t *chunk);

//static void merge_chunk(header_t *chunk);

static uint32_t heap_max = HEAP_START;

static header_t *heap_first;

/*
Virtual Address Space from 0xE0000000 ++
+------------------------------------------------------------------------------------------+
+                                                                                          +
+                         Free Virtual Address Space                                       +
+                                                                                          +
+                                                                                          +
+------------------------------------------------------------------------------------------+
------> direction malloc
<------ direction free

+-----------------------------+
+               +             +
+               +             +
+add chunk_info + Size        +
+               +             +
+-----------------------------+
va

malloc(50)
1. check if heap has enough 50 bytes space
2. Get Physical Address Page Unit 4K
3. Map (va to pa) -> 4K Ram To this Va (Heap_max) ->every old Heap_max memory map to pa and new Heap_max
   is heap_top
4. chunk info = prev, next, allocated
5. connect to prev
6. malloc return address always pointer to len start w/o chunk info block

Physical RAM

free(pointer)
1. get pointer = va
2. move pointer to the begining of chunk p - sizeof(header_t)
3. allocated = 0
4. check chunk surrounding chunks if they are unallocated
    if there is meger to one chunk
5. if chunk size larger than page size upmap page

malloc
1. first check current chunks has free one allocated to 0
    if has go split function
2. heap management chunk, insert

*/
void init_heap(){
  heap_first = 0;
}

void *kmalloc(uint32_t len){
  len += sizeof(header_t);

  header_t *cur_header = heap_first;
  header_t *prev_header = 0;

  while(cur_header){
    if(cur_header->allocated == 0 && cur_header->length >= len){
      split_chunk(cur_header, len);
      cur_header->allocated = 1;
      return (void *)((uint32_t)cur_header + sizeof(header_t));
    }

    prev_header = cur_header;
    cur_header = cur_header->next;
  }

  uint32_t chunk_start;

  if(prev_header){
    chunk_start = (uint32_t)prev_header + prev_header->length;
  }
  else{
    chunk_start = HEAP_START;
    heap_first = (header_t *)chunk_start;
  }

  //check if need RAM
  alloc_chunk(chunk_start, len);
  cur_header = (header_t *)chunk_start;
  cur_header->prev = prev_header;
  cur_header->next = 0;
  cur_header->allocated = 1;
  cur_header->length = len;

  if(prev_header)
    prev_header->next = cur_header;

  return (void*)(chunk_start + sizeof(header_t));
}

void kfree(void *p){
  header_t *header = (header_t *)((uint32_t)p -sizeof(header_t));
  header->allocated = 0;

  glue_chunk(header);
}

void alloc_chunk(uint32_t chunk_start, uint32_t len){
  //left_size < request size -> need require another 4K page Physical RAM
  //printk("Mapping called\n");
  while(chunk_start + len > heap_max){
    uint32_t page = pmm_alloc_page();
    map(pgd_kern, heap_max, page, PAGE_PRESENT|PAGE_WRITE);

    if((heap_max + PAGE_SIZE) <= HEAP_VIRTUAL_MAX)
      heap_max += PAGE_SIZE;
  }
}

void split_chunk(header_t *chunk, uint32_t len){
  //get new free chunk after split
  //split current free chunk partially and return the new free chunk with left size
  //printk("Split called\n");
  if(chunk->length - len >= sizeof(header_t)){
    header_t * newchunk = (header_t *)((uint32_t)chunk + chunk->length);
    newchunk->prev = chunk;
    newchunk->next = chunk->next;
    newchunk->allocated = 0;
    newchunk->length = chunk->length - len;

    //previous chunk are split, this part are used for new malloc
    chunk->next = newchunk;
    chunk->length = len;
  }
}

void glue_chunk(header_t * chunk){
  //glue together closed unused chunks
  //next
  if(chunk->next && chunk->next->allocated == 0){
    //printk("Glue Called\n");
    chunk->length = chunk->length + chunk->next->length;
    if(chunk->next->next){
      chunk->next->next->prev = chunk;
    }
    chunk->next = chunk->next->next;
  }
  //previous
  if(chunk->prev && chunk->prev->allocated == 0){
    //printk("Glue Called\n");
    chunk->prev->length = chunk->prev->length + chunk->length;
    chunk->prev->next = chunk->next;

    if(chunk->next){
      chunk->next->prev = chunk->prev;
    }
    chunk = chunk->prev;
  }

  if(chunk->next == 0){
    //printk("Glue Called and Unmap called\n");
    free_chunk(chunk);
  }
}

void free_chunk(header_t *chunk){
  //no allocated chunk all free
  //only move pointer saying this location is used, no pointed are all free could be filled with
  //something but already free, so no use and see as empty and free
  if(chunk->prev == 0)
    heap_first = 0;
  else
    chunk->prev->next = 0;

  //if all free ram over 4K page, then unmap to let system run efficient
  while((heap_max - PAGE_SIZE) >= (uint32_t) chunk){
      heap_max -= PAGE_SIZE;
      uint32_t page;
      get_mapping(pgd_kern, heap_max, &page);
      unmap(pgd_kern, heap_max);
      pmm_free_page(page);
  }

}

/*void merge_chunk(header_t *chunk){
  if(chunk->prev){

  }

  if(chunk->next){

  }


}*/

void test_heap(){
  printk_color(black, light_magenta, "Test kmalloc() && kfree()\n");

  void *addr1 = kmalloc(100);
  printk_color(black, light_cyan, "kmalloc 100 bytes in 0x%x\n", addr1);

  void *addr2 = kmalloc(10);
  printk_color(black, light_cyan, "kmalloc 10 bytes in 0x%x\n", addr2);

  kfree(addr1);
  printk_color(black, light_cyan, "kfree 100 bytes in 0x%x\n", addr1);

  void *addr3 = kmalloc(30);
  printk_color(black, light_cyan, "kmalloc 30 bytes in 0x%x\n", addr3);

  printk("\n");

  kfree(addr2);
  printk_color(black, light_cyan, "kfree 10 bytes in 0x%x\n", addr2);

  kfree(addr3);
  printk_color(black, light_cyan, "kfree 30 bytes in 0x%x\n", addr3);
}
