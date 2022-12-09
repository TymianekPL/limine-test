#include "ports.h"
#include "debug.h"

/**
 * Read a byte from the specified port
 */
uint8_t port_byte_in(uint16_t port)
{
    uint8_t result;
    /* Inline assembler syntax
     * !! Notice how the source and destination registers are switched from NASM !!
     *
     * '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
     * '"d" (port)': map the C variable '(port)' into e'd'x register
     *
     * Inputs and outputs are separated by colons
     */
    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the __asm__ syntax
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    __asm__ volatile("out %%al, %%dx"
                     :
                     : "a"(data), "d"(port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;
    __asm__("in %%dx, %%ax"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    __asm__ volatile("out %%ax, %%dx"
                     :
                     : "a"(data), "d"(port));
}

// outportsm
void outportsm(unsigned short port, unsigned char *data, unsigned long size)
{
    __asm__ volatile("rep outsw"
                     : "+S"(data), "+c"(size)
                     : "d"(port));
}

// inportb
uint8_t inportsm(uint16_t port)
{
    uint8_t result;
    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

// inports
uint16_t inports(uint16_t port)
{
    uint16_t result;
    __asm__("in %%dx, %%ax"
            : "=a"(result)
            : "d"(port));
    return result;
}