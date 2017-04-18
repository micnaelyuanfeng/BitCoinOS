#ifndef _INCLUDE_IDT_H_
#define _INCLUDE_IDT_H_

#include "types.h"

#define ISR120 120
#define ISR121 121

#define IRQ_OFFSET 32

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct idt_entry_t{
  //address of interrupt handler function
  uint16_t base_lo;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_hi;
}__attribute__((packed))idt_entry_t;

typedef struct idt_ptr_t{
  uint16_t limit;
  uint32_t base;
}__attribute__((packed))idt_ptr_t;

typedef struct pt_regs_t{
    uint32_t ds;  //data segment

    // pusha on stack all
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t int_no;
    uint32_t err_code;

    //CPU push on stack automatically
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    //CPU push on stack and for user
    uint32_t useresp;
    uint32_t ss;
}pt_regs;

idt_entry_t idt_entries[256];

idt_ptr_t idt_ptr;

typedef void (* interrupt_handler_t)(pt_regs *);

interrupt_handler_t interrupt_handers[256];

void init_idt(void);

void register_interrupt_handler(uint8_t n, interrupt_handler_t h);

void isr_handler(pt_regs *regs);

extern void idt_flush(uint32_t);

void isr_handler(pt_regs * regs);

void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);
void isr255(void);

//user defined
void isr120(void);
void isr121(void);

void irq_hander(pt_regs *regs);

void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

/**********************
8259A Definition Block

                  +----------+
                  + IR0   Int+--------------CPU int pin
                  +   .      +
                  +   .      + Master
                  + IR7      +
                  +----------+

                  +----------+
                  +          + Salve
                  +          +
                  +          +
                  +          +
                  +----------+

Pheripherals to 8259A -> interrupt requests
All Pheripherals interrupt requests goes to 8259A

0 - 31 reserved
IRQ0 is timer       32
IRQ1 is keyboard    33
 .
 .
 .
IRQ14 is IDE0       46
IRQ15 is IDE1       47


**********************/
#endif
