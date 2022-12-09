#include "isr.h"
#include "ports.h"

idt_entry idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint64_t handler)
{
    idt[n].offset_low = low_16(handler);
    idt[n].offset_mid - handler >> 16;
    idt[n].offset_hi = handler >> 32;
    idt[n].selector = KERNEL_CS;
    idt[n].flags = 0x8E;
}

void set_idt()
{
    idt_reg.base = (uint64_t *)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_entry) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ volatile("lidt %0"
                     :
                     : "memory"(idt_reg));
}

isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install()
{
    set_idt_gate(0, (uint64_t)isr0);
    set_idt_gate(1, (uint64_t)isr1);
    set_idt_gate(2, (uint64_t)isr2);
    set_idt_gate(3, (uint64_t)isr3);
    set_idt_gate(4, (uint64_t)isr4);
    set_idt_gate(5, (uint64_t)isr5);
    set_idt_gate(6, (uint64_t)isr6);
    set_idt_gate(7, (uint64_t)isr7);
    set_idt_gate(8, (uint64_t)isr8);
    set_idt_gate(9, (uint64_t)isr9);
    set_idt_gate(10, (uint64_t)isr10);
    set_idt_gate(11, (uint64_t)isr11);
    set_idt_gate(12, (uint64_t)isr12);
    set_idt_gate(13, (uint64_t)isr13);
    set_idt_gate(14, (uint64_t)isr14);
    set_idt_gate(15, (uint64_t)isr15);
    set_idt_gate(16, (uint64_t)isr16);
    set_idt_gate(17, (uint64_t)isr17);
    set_idt_gate(18, (uint64_t)isr18);
    set_idt_gate(19, (uint64_t)isr19);
    set_idt_gate(20, (uint64_t)isr20);
    set_idt_gate(21, (uint64_t)isr21);
    set_idt_gate(22, (uint64_t)isr22);
    set_idt_gate(23, (uint64_t)isr23);
    set_idt_gate(24, (uint64_t)isr24);
    set_idt_gate(25, (uint64_t)isr25);
    set_idt_gate(26, (uint64_t)isr26);
    set_idt_gate(27, (uint64_t)isr27);
    set_idt_gate(28, (uint64_t)isr28);
    set_idt_gate(29, (uint64_t)isr29);
    set_idt_gate(30, (uint64_t)isr30);
    set_idt_gate(31, (uint64_t)isr31);

    // Remap the PIC
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    // Install the IRQs
    set_idt_gate(32, (uint64_t)irq0);
    set_idt_gate(33, (uint64_t)irq1);
    set_idt_gate(34, (uint64_t)irq2);
    set_idt_gate(35, (uint64_t)irq3);
    set_idt_gate(36, (uint64_t)irq4);
    set_idt_gate(37, (uint64_t)irq5);
    set_idt_gate(38, (uint64_t)irq6);
    set_idt_gate(39, (uint64_t)irq7);
    set_idt_gate(40, (uint64_t)irq8);
    set_idt_gate(41, (uint64_t)irq9);
    set_idt_gate(42, (uint64_t)irq10);
    set_idt_gate(43, (uint64_t)irq11);
    set_idt_gate(44, (uint64_t)irq12);
    set_idt_gate(45, (uint64_t)irq13);
    set_idt_gate(46, (uint64_t)irq14);
    set_idt_gate(47, (uint64_t)irq15);

    set_idt(); // Load with __asm__
}

void isr_handler(registers_t *r)
{
    /* To print the message which defines every exception */
    char *exception_messages[32] = {
        "Division By Zero",       // 1
        "Debug",                  // 2
        "Non Maskable Interrupt", // 3
        "Breakpoint",             // 4
        "Into Detected Overflow", // 5
        "Out of Bounds",          // 6
        "Invalid Opcode",         // 7
        "No Coprocessor",         // 8

        "Double Fault",                // 9
        "Coprocessor Segment Overrun", // 10
        "Bad TSS",                     // 11
        "Segment Not Present",         // 12
        "Stack Fault",                 // 13
        "General Protection Fault",    // 14
        "Page Fault",                  // 15
        "Unknown Interrupt",           // 16

        "Coprocessor Fault", // 17
        "Alignment Check",   // 18
        "Machine Check",     // 19
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",

        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"};
    if (exception_messages[r->int_no] != 0)
    {
        char *message = (exception_messages[r->int_no]);

        error(message, strlen(message));
    }
}

void RegisterInterruptHandler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t *r)
{
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r->int_no >= 40)
        port_byte_out(0xA0, 0x20); /* slave */
    port_byte_out(0x20, 0x20);     /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
}

void irq_install()
{
    /* Enable interruptions */
    /* IRQ0: timer */
    // init_timer(100);
    /* IRQ1: keyboard */
    InitializeKeyboard();
}