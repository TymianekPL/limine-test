#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "debug.h"

/* Segment selectors */
#define KERNEL_CS 0x28

#define low_16(address) (uint16_t)((address)&0xffff)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xffff)

typedef struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t reserved;
} idt_entry;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */
typedef struct
{
    uint64_t base;
    uint16_t limit;
} __attribute__((packed)) idt_register_t;

#ifndef ISRTEST_H
#define ISRTEST_H
#define IDT_ENTRIES 256
extern idt_entry idt[IDT_ENTRIES];
extern idt_register_t idt_reg;
#endif

/* Functions implemented in idt.c */
void set_idt_gate(int n, uint64_t handler);
void set_idt();

/* ISRs reserved for CPU exceptions */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
/* IRQ definitions */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

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

/* Struct which aggregates many registers.
 * It matches exactly the pushes on interrupt.asm. From the bottom:
 * - Pushed by the processor automatically
 * - `push byte`s on the isr-specific code: error code, then int number
 * - All the registers by pusha
 * - `push rax` whose lower 16-bits contain DS
 */
typedef struct
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t int_no, error_code, rip, cs, rflags, rsp, ss;
} __attribute__((packed)) registers_t;

void isr_install();
void isr_handler(registers_t *r);
void irq_install();
typedef void (*isr_t)(registers_t *);
void RegisterInterruptHandler(uint8_t n, isr_t handler);

#define panic(...)                                                         \
    {                                                                      \
        error("KERNEL PANIC in %s:%d@%s\n", __FILE__, __LINE__, __func__); \
        error(__VA_ARGS__);                                                \
        __asm__ volatile("cli");                                           \
        for (;;)                                                           \
            __asm__ volatile("hlt");                                       \
    }

#endif
