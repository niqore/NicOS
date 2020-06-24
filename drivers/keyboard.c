#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../cpu/types.h"
#include "../kernel/utils.h"

static void keyboard_callback(registers_t regs) {

    /* Le PIC met le scancode dans 0x60 */
    uint8_t scancode = port_byte_in(0x60);
    unsigned char sc_ascii[4];
    itoa(scancode, sc_ascii, 4, 16);
    print_string("Keyboard scancode: 0x");
    print_string((char *) sc_ascii);
    print_string("\n");
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback); 
}