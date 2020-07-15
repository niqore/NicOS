#include "../drivers/screen.h"
#include "../cpu/interrupts/isr.h"
#include "../cpu/interrupts/pic.h"
#include "cli.h"
#include "../libc/stdlib.h"
#include "memory_map.h"
#include "kernel.h"
#include "../drivers/pci.h"

void main() {

	clear_screen();
	print_string("Bienvenue sur NicOS !\nLe CPU est actuellement en mode 32-bit\n\n");

	isr_install();

	if (apic_available()) {
		enable_apic();
	}

	irq_install();

	init_cli();

	init_memory_allocator((SMAP_entry_t*) MEM_MAP_STRUCTS_ADDR, *((uint32_t*) MEM_MAP_ENT_ADDR));

	scan_and_register_devices();

	while (1);
}