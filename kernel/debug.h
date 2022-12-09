#ifndef DEBUG_H
#define DEBUG_H
#include <limine.h>
#include <stdint.h>

void debug_init(struct limine_terminal_request term);

void info(char *message, uint64_t size);
void ok(char *message, uint64_t size);
void warn(char *message, uint64_t size);
void error(char *message, uint64_t size);

void cout(char chr);

#endif