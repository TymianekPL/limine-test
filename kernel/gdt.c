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

// void SetSegmentDescriptor(SegmentDescriptor *segment, uint32_t base,
//                           uint32_t limit, uint8_t type)
// {
//     uint8_t *chunked = (uint8_t *)segment;

//     if (limit < (1 << 16))
//     {
//         chunked[6] = 0x40;
//     }
//     else
//     {
//         if ((limit & 0xFFF) != 0xFFF)
//             limit = (limit >> 12) - 1;
//         else
//             limit = limit >> 12;

//         chunked[6] = 0xC0;
//     }

//     // Encode the limit
//     chunked[0] = limit & 0xFF;
//     chunked[1] = (limit >> 8) & 0xFF;
//     chunked[6] |= (limit >> 16) & 0xF;

//     // Encode the base
//     chunked[2] = base & 0xFF;
//     chunked[3] = (base >> 8) & 0xFF;
//     chunked[4] = (base >> 16) & 0xFF;
//     chunked[7] = (base >> 24) & 0xFF;

//     // Type
//     chunked[5] = type;
// }

// uint32_t GetBaseFromSegment(SegmentDescriptor *segment)
// {
//     uint8_t *chunked = (uint8_t *)segment;
//     uint32_t base = chunked[7];
//     base = (base << 8) + chunked[4];
//     base = (base << 8) + chunked[3];
//     base = (base << 8) + chunked[2];

//     return base;
// }

// uint32_t GetLimitFromSegment(SegmentDescriptor *segment)
// {
//     uint8_t *chunked = (uint8_t *)segment;

//     uint32_t limit = chunked[6] & 0xF;
//     limit = (limit << 8) + chunked[1];
//     limit = (limit << 8) + chunked[0];

//     if ((chunked[6] & 0xC0) == 0xC0)
//         limit = (limit << 12) | 0xFFF;

//     return limit;
// }

// uint8_t GetTypeFromSegment(SegmentDescriptor *segment)
// {
//     uint8_t *chunked = (uint8_t *)segment;
//     return chunked[5];
// }

// void InitializeGDT(GlobalDescriptorTable *gdt_ptr)
// {
//     SetSegmentDescriptor(&(gdt_ptr->nullSegmentSelector), 0, 0, 0);
//     SetSegmentDescriptor(&(gdt_ptr->kernelCode64SegmentSelector), 0, 0, 0);
//     SetSegmentDescriptor(&(gdt_ptr->kernelDataSelector), 0, 64 * 1024 * 1024, 0x9A);
//     SetSegmentDescriptor(&(gdt_ptr->userDataSegmentSelector), 0, 64 * 1024 * 1024, 0x92);
//     SetSegmentDescriptor(&(gdt_ptr->userCode64SegmentSelector), 0, 64 * 1024 * 1024, 0x92);
//     int n_gdt_entries = 5;

//     struct
//     {
//         uint32_t ptr;
//         uint16_t size;
//     } __attribute__((packed)) ptr = {0};
//     ptr.ptr = (uint32_t)gdt_ptr;
//     ptr.size = (n_gdt_entries * 8) - 1;

//     __asm__ volatile("lgdt %0"
//                      :
//                      : "m"(ptr));
//     __asm__ volatile("mov %ax, 0x10            \t\n\
//                   mov %ds, %ax              \t\n\
//                   mov %es, %ax              \t\n\
//                   mov %fs, %ax              \t\n\
//                   mov %gs, %ax              \t\n\
//                   mov %ss, %ax              \t\n\
//         ");
// }