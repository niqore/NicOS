#include "multiboot2.h"

#include "../drivers/screen.h"
#include "../cpu/interrupts/isr.h"
#include "../cpu/interrupts/pic.h"
#include "cli.h"
#include "../libc/stdlib.h"
#include "../libc/stdio.h"
#include "../drivers/pci.h"
#include "../drivers/ahci.h"
#include "../filesystem/fat32.h"
#include "../filesystem/filesystem.h"
#include "../libc/random.h"
#include "../filesystem/elf.h"

void main() {

	/* Retrieve Multiboot values */
	uint32_t bootloader_magic;
	asm("\t movl %%eax,%0" : "=r"(bootloader_magic));
	uint32_t multiboot_info_struct_addr;
	asm("\t movl %%ebx,%0" : "=r"(multiboot_info_struct_addr));

	/* GDT */
	extern void set_gdt();
	set_gdt();

	/* Clear the screen */
	clear_screen();

	/* Verify multiboot information */
	if (bootloader_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		printf("Bad multiboot2 magic number. Halting...");
		__asm__ __volatile__("hlt");
	}
	if (multiboot_info_struct_addr & 7) {
		printf("Unaligned Multiboot Information. Halting...");
		__asm__ __volatile__("hlt");
	}

	/* Read tags */
	for (struct multiboot_tag *tag = (struct multiboot_tag *) (multiboot_info_struct_addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
    	switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				printf("Booted with %s\n", ((struct multiboot_tag_string *) tag)->string);
				break;
          	case MULTIBOOT_TAG_TYPE_BOOTDEV:
          		printf ("Boot device 0x%x,%u,%u\n",
                  ((struct multiboot_tag_bootdev *) tag)->biosdev,
                  ((struct multiboot_tag_bootdev *) tag)->slice,
                  ((struct multiboot_tag_bootdev *) tag)->part);
          		break;
        	case MULTIBOOT_TAG_TYPE_MODULE:
          		printf ("Module at 0x%x-0x%x. Command line %s\n",
                  ((struct multiboot_tag_module *) tag)->mod_start,
                  ((struct multiboot_tag_module *) tag)->mod_end,
                  ((struct multiboot_tag_module *) tag)->cmdline);
          		break;
          	case MULTIBOOT_TAG_TYPE_MMAP:
          		init_memory_allocator(tag);
          		break;
        }
    }

	/* Welcome ! */
	printf("\nBienvenue sur NicOS !\nLe CPU est actuellement en mode 32-bit\n\n");

	/* Interruptions */
	isr_install();
	if (apic_available()) {
		enable_apic();
	}
	irq_install();

	/* Devices and disk */
	scan_and_register_pci_devices();
	ahci_pci_header* ahci_disk = (ahci_pci_header*) get_ahci_disk();
	if (!ahci_disk) {
		printf("AHCI Disk not found. Halting...");
		__asm__ __volatile__("hlt");
	}
	register_ahci_disk(ahci_disk);

	/* Filesystem initialization */
	init_fat32_filesystem();

	/* Command Line Interface */
	init_cli();

	while (1) {
		if(execute_elf_queue()) {
			init_cli();
		}
	}
}