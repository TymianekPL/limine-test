#include "debug.h"

struct limine_terminal_request terminal_request;

void debug_init(struct limine_terminal_request term)
{
    terminal_request = term;

    struct limine_terminal* terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "\e[48;2;17;17;17m\e[2J\e[H", 23);
}

void info(char* message, uint64_t size)
{
    struct limine_terminal* terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "\e[1m\e[48;2;17;17;17m\e[38;2;80;150;240m", 38);
    terminal_request.response->write(terminal, "[INF] ", 6);
    terminal_request.response->write(terminal, message, size);
    terminal_request.response->write(terminal, "\n\e[0m", 5);
}

void ok(char* message, uint64_t size)
{

    struct limine_terminal* terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "\e[1m\e[48;2;17;17;17m\e[38;2;80;255;80m", 38);
    terminal_request.response->write(terminal, "[OK] ", 6);
    terminal_request.response->write(terminal, message, size);
    terminal_request.response->write(terminal, "\n\e[0m", 6);
}

void warn(char* message, uint64_t size)
{

    struct limine_terminal* terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "\e[1m\e[48;2;17;17;17m\e[38;2;250;250;80m", 39);
    terminal_request.response->write(terminal, "[WARN] ", 8);
    terminal_request.response->write(terminal, message, size);
    terminal_request.response->write(terminal, "\n\e[0m", 6);
}

void error(char* message, uint64_t size)
{

    struct limine_terminal* terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "\e[1m\e[48;2;17;17;17m\e[38;2;80;250;80m", 39);
    terminal_request.response->write(terminal, "[ERR] ", 7);
    terminal_request.response->write(terminal, message, size);
    terminal_request.response->write(terminal, "\n\e[0m", 6);
}

void cout(char chr)
{
    struct limine_terminal* terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, &chr, 1);
}