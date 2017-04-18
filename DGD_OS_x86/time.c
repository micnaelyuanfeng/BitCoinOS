#include "time.h"
#include "debug.h"
#include "comman.h"
#include "idt.h"
#include "task.h"
#include "sched.h"

void timer_callback(pt_regs *reg){
  //static uint32_t tick = 0;
  //printk_color(black, white, "Tick: %d\r", tick++);
  schedule();
}

void init_timer(uint32_t frequency){
  register_interrupt_handler(IRQ0, timer_callback);

  // # of interrupt per sec
  uint32_t divisor = 1193180 / frequency;

  //Intel 8253 and 8254 timer
  //I/O direct memory map is 040h - 043h
  outb(0x43, 0x36);

  uint8_t low = (uint8_t)(divisor & 0xFF);
  uint8_t high = ((uint8_t) ((divisor >> 8) & 0xFF));

  outb(0x40, low);
  outb(0x40, high);
}
