#ifndef _INCLUDE_COMMON_H_
#define _INCLUDE_COMMON_H_

#include "types.h"

void outb(uint16_t port, uint8_t value);

uint8_t inb (uint16_t port);

uint16_t inw (uint16_t port);

void insl(uint32_t port, void * addr, uint32_t n);

void outsl(uint32_t port, void * addr, uint32_t n);

void enable_intr(void);

void disable_intr(void);

void cpu_hlt(void);

#endif
