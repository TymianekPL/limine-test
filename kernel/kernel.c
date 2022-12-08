#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "debug.h"
#include "gdt.h"

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0};
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

static void done(void)
{
    for (;;)
    {
        __asm__("hlt");
    }
}

void putPixel(int x, int y, uint32_t pixel)
{
    struct limine_framebuffer *buffer = framebuffer_request.response->framebuffers[0];
    *((uint32_t *)(buffer->address + 4 * (buffer->pitch / 4) * y + 4 * x)) = pixel;
}

void fillSquare(int x, int y, int volX, int volY, uint32_t color)
{
    for (int i = y; i < y + volY; i++)
    {
        for (int j = x; j < x + volX; j++)
        {
            putPixel(j, i, color);
        }
    }
}

extern void disablePIC();

static GDTDescriptor gdtDescriptor;
// The following will be our kernel's entry point.
void _start(void)
{
    // Ensure we got a terminal
    if (terminal_request.response == NULL || terminal_request.response->terminal_count < 1)
    {
        done();
    }
    struct limine_framebuffer *buffer = framebuffer_request.response->framebuffers[0];
    debug_init(terminal_request);
    fillSquare(0, 0, buffer->width, buffer->height, 0x111111);

    info("Initializing...", 15);
    gdtDescriptor.Size = sizeof(GDT);
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    struct limine_terminal *terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "Test", 4);

    // We're done, just hang...
    done();
}
