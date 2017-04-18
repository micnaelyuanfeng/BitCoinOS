#include "comman.h"

inline void outb(uint16_t port, uint8_t value){
  asm volatile ("outb %1, %0" : : "dN" (port), "a"(value));
}

inline uint8_t inb(uint16_t port){
  uint8_t ret;

  asm volatile("inb %1, %0" :  "=a"(ret) : "dN"(port));

  return ret;
}

inline uint16_t inw(uint16_t port){
  uint16_t ret;

  asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port) );

  return ret;
}

void insl(uint32_t port, void * addr, uint32_t n){
  asm volatile ("cld");
  asm volatile ("repne \n"
                "insl"
                :"=D"(addr), "=c"(n)
                :"d"(port), "0"(addr), "l"(n)
                : "memory", "cc");
}

void outsl(uint32_t port, void * addr, uint32_t n){
  asm volatile ("cld");
  asm volatile ("repne \n"
                "outsl"
                :"=D"(addr), "=c"(n)
                :"d"(port), "0"(addr), "l"(n)
                : "memory", "cc");
}

void enable_intr(){
  asm volatile ("sti");
}

void disable_intr(){
  asm volatile ("cli");
}

void cpu_hlt(){
  asm volatile ("hlt");
}
