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

// typedef struct SegmentDescriptor_t
// {
//     uint16_t limit_lo;
//     uint16_t base_lo;
//     uint8_t base_hi;
//     uint8_t type;
//     uint8_t flags_limit_hi;
//     uint8_t base_vhi;
// } __attribute__((packed)) SegmentDescriptor;

// struct SegmentDescriptor *GetSegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
// void SetSegmentDescriptor(SegmentDescriptor *segment, uint32_t base, uint32_t limit, uint8_t type);

// uint32_t GetBaseFromSegment(SegmentDescriptor *segment);
// uint32_t GetLimitFromSegment(SegmentDescriptor *segment);
// uint8_t GetTypeFromSegment(SegmentDescriptor *segment);

// typedef struct GlobalDescriptorTable_t
// {
//     SegmentDescriptor nullSegmentSelector;
//     SegmentDescriptor kernelCode64SegmentSelector;
//     SegmentDescriptor kernelDataSelector;
//     SegmentDescriptor userDataSegmentSelector;
//     SegmentDescriptor userCode64SegmentSelector;
// } __attribute__((packed)) GlobalDescriptorTable;

// void InitializeGDT(GlobalDescriptorTable *gdt);

#endif