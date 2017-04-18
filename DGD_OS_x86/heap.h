#ifndef _HEAP_H_
#define _HEAP_H_

#include "types.h"

#define HEAP_START 0xE0000000
#define HEAP_VIRTUAL_MAX 0xFFFFFFFF

//ram management unit
typedef struct header{
  struct header *prev;
  struct header *next;
  uint32_t allocated : 1; //bit field
  uint32_t length : 31; //length of ram chunk
}header_t;

void init_heap();

void *kmalloc(uint32_t len);

void kfree(void *p);

void test_heap();

#endif
