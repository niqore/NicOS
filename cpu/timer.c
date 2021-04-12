#include "timer.h"
#include "../drivers/screen.h"
#include "interrupts/isr.h"
#include "ports.h"
#include "../libc/stdio.h"

uint32_t tick = 0;

uint32_t get_current_tick() {
    return tick;
}

static void timer_callback(registers_t regs) {

    regs = regs;
    tick++;
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

void sleep(uint32_t ms) {
    uint32_t lim = tick + ms;
    while (tick < lim);
}