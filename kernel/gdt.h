#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

typedef struct GDTDescriptor
{
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed)) GDTDescriptor;

typedef struct GDTEntry
{
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
} __attribute__((packed)) GDTEntry;

typedef struct GDT
{
    GDTEntry Null;       // 0x00
    GDTEntry KernelCode; // 0x28
    GDTEntry KernelData; // 0x30
    GDTEntry UserNull;
    GDTEntry UserCode;
    GDTEntry UserData;
} __attribute__((packed))
__attribute((aligned(0x1000))) GDT;

extern GDT DefaultGDT;

extern void LoadGDT(GDTDescriptor *gdtDescriptor);

void InitializeGDT(uint64_t *gdt);

#endif