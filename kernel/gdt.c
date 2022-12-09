#include <gdt.h>
#include <stdint.h>

__attribute__((aligned(0x1000)))
GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0}, // null
    {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // kernel data segment
    {0, 0, 0, 0x00, 0x00, 0}, // user null
    {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // kernel data segment
};

void InitializeGDT(uint64_t *gdt_ptr)
{
    int n_gdt_entries = 9;

    struct
    {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) ptr = {0};
    ptr.base = (uint64_t)gdt_ptr;
    ptr.limit = (n_gdt_entries * sizeof(uint64_t)) - 1;

    LoadGDT(&ptr);
}