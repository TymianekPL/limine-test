#include "keyboard.h"
#include "isr.h"
#include <stdint.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

#define SC_MAX 57
const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

void user_input(char *message)
{
}

static void
KeyboardCallback(registers_t *regs)
{
    ok("lol", 3);
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX)
        return;
    if (scancode == BACKSPACE && strlen(key_buffer) > 0)
    {
        backspace(key_buffer);
        cout('\b');
    }
    else if (scancode == ENTER)
    {
        cout('\n');
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
    }
    else
    {
        char letter = sc_ascii[(int)scancode];
        if (letter != '?')
        {
            /* Remember that kprint only accepts char[] */
            char str[2] = {letter, '\0'};
            append(key_buffer, letter);
            cout(letter);
        }
    }
    (void)(regs);
}

void InitializeKeyboard(void)
{
    RegisterInterruptHandler(IRQ1, KeyboardCallback);
}