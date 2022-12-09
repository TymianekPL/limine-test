#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "debug.h"

/* Segment selectors */
#define KERNEL_CS 0x28

#define low_16(address) (uint16_t)((address)&0xffff)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xffff)

struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t reserved;
};

/* How every interrupt gate (handler) is defined */
typedef struct
{
    uint16_t low_offset; /* Lower 16 bits of handler function address */
    uint16_t sel;        /* Kernel segment selector */
    uint8_t always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    uint8_t flags;
    uint16_t high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t;

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
extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_reg;
#endif

/* Functions implemented in idt.c */
void set_idt_gate(int n, uint32_t handler);
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
 * - `push eax` whose lower 16-bits contain DS
 */
typedef struct
{
    uint32_t ds;                                         /* Data segment selector */
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; /* Pushed by pusha. */
    uint32_t int_no, err_code;                           /* Interrupt number and error code (if applicable) */
    uint32_t eip, cs, eflags, esp, ss;                   /* Pushed by the processor automatically */
} registers_t;

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
