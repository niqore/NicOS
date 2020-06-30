void main_16bit() __attribute__((section(".start")));

#include "../drivers/screen.h"
#include "../cpu/interrupts/isr.h"
#include "../cpu/interrupts/pic.h"
#include "cli.h"
#include "../libc/stdlib.h"

void main_16bit() {

	extern void switch_to_pm();
	switch_to_pm();
}

void main() {

	clear_screen();
	print_string("Bienvenue sur NicOS !\nLe CPU est actuellement en mode 32-bit\n\n");

	isr_install();

	if (apic_available()) {
		enable_apic();
	}

	irq_install();

	init_cli();

	init_memory_allocator();
}