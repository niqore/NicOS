#include "multiboot2.h"

#include "../drivers/screen.h"
#include "../cpu/interrupts/isr.h"
#include "../cpu/interrupts/pic.h"
#include "cli.h"
#include "../libc/stdlib.h"
#include "memory_map.h"
#include "kernel.h"
#include "../libc/stdio.h"

void main() {

	uint32_t bootloader_magic;
	asm("\t movl %%eax,%0" : "=r"(bootloader_magic));
	uint32_t multiboot_info_struct_addr;
	asm("\t movl %%ebx,%0" : "=r"(multiboot_info_struct_addr));

	//multiboot_info_t * mbi = (multiboot_info_t *) addr;

	extern void set_gdt();
	set_gdt();

	clear_screen();

	if (bootloader_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		printf("Bad multiboot2 magic number. Halting...");
		__asm__ __volatile__("hlt");
	}

	printf("Bienvenue sur NicOS !\nLe CPU est actuellement en mode 32-bit\n\n");

	isr_install();

	if (apic_available()) {
		enable_apic();
	}

	irq_install();

	init_cli();

	/*init_memory_allocator((SMAP_entry_t*) MEM_MAP_STRUCTS_ADDR, *((uint32_t*) MEM_MAP_ENT_ADDR));

	//TODO set esp (stack)

	device_count = count_pci_devices();
	device_list = (generic_pci_device_header_t*) malloc(device_count * sizeof(generic_pci_device_header_t));
	scan_and_register_pci_devices(device_list);*/

	while (1);
}