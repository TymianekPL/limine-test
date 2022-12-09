#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>


unsigned char port_byte_in (uint16_t port);
void port_byte_out (uint16_t port, uint8_t data);
unsigned short port_word_in (uint16_t port);
void port_word_out (uint16_t port, uint16_t data);
void outportsm(unsigned short port, unsigned char* data, unsigned long size);
uint16_t inports(uint16_t port);

#define outb(port, data) port_byte_out(port, data)
#define inb(port) port_byte_in(port)
#define outw(port, data) port_word_out(port, data)
#define inw(port) port_word_in(port)

#endif
