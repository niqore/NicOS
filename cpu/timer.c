#include "timer.h"
#include "../drivers/screen.h"
#include "../kernel/utils.h"
#include "isr.h"
#include "../drivers/ports.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {

    tick++;
    print_string("Tick: ");
    
    unsigned char tick_ascii[256];
    itoa(tick, tick_ascii, 256, 10);
    print_string((char *) tick_ascii);
    print_char('\n');
}

void init_timer(uint32_t freq) {

    /* Enregistrement du callback */
    register_interrupt_handler(IRQ0, timer_callback);

    /* On calcule le PIT (Programmable Interval Timer). Voir https://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interval_Timer */
    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    /* Envoie de la commande */
    port_byte_out(0x43, 0x36); /* Port de commande */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}