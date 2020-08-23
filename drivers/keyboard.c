/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Nicolas Virard <nicolas.virard@ecole.ensicaen.fr> 
 * @version     0.0.1 - 23-08-2020
 */

#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/interrupts/isr.h"
#include "screen.h"
#include "../cpu/types.h"
#include "../kernel/cli.h"

static void keyboard_callback(registers_t regs) {

	regs = regs;

    /* Le PIC met le scancode dans 0x60 */
    uint8_t scancode = port_byte_in(0x60);
    /*unsigned char sc_ascii[4];
    itoa(scancode, sc_ascii, 4, 16);
    print_string("Keyboard scancode: 0x");
    print_string((char *) sc_ascii);
    print_string("\n");*/

    handle_key(scancode);
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback); 
}