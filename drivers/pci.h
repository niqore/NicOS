#ifndef PCI_H
#define PCI_H

#include "../cpu/types.h"

typedef struct generic_pci_device_header {
 
	uint16_t vendor_id;
	uint16_t device_id;
	/*
	bit 15 to 11: reserved
	bit 10: Interrupt Disable - If set to 1 the assertion of the devices INTx# signal is disabled; otherwise, assertion of the signal is enabled. 
	bit 9: Fast Back-Back Enable - If set to 1 indicates a device is allowed to generate fast back-to-back transactions; otherwise, fast back-to-back transactions are only allowed to the same agent.
	bit 8: SERR# Enable - If set to 1 the SERR# driver is enabled; otherwise, the driver is disabled.
	bit 7: reserved
	bit 6: Parity Error Response - If set to 1 the device will take its normal action when a parity error is detected; otherwise, when an error is detected, the device will set bit 15 of the Status register (Detected Parity Error Status Bit), but will not assert the PERR# (Parity Error) pin and will continue operation as normal.
	bit 5: VGA Palette Snoop - If set to 1 the device does not respond to palette register writes and will snoop the data; otherwise, the device will trate palette write accesses like all other accesses.
	bit 4: Memory Write and Invalidate Enable - If set to 1 the device can generate the Memory Write and Invalidate command; otherwise, the Memory Write command must be used.
	bit 3: Special Cycles - If set to 1 the device can monitor Special Cycle operations; otherwise, the device will ignore them.
	bit 2: Bus Master - If set to 1 the device can behave as a bus master; otherwise, the device can not generate PCI accesses.
	bit 1: Memory Space - If set to 1 the device can respond to Memory Space accesses; otherwise, the device's response is disabled.
	bit 0: I/O Space - If set to 1 the device can respond to I/O Space accesses; otherwise, the device's response is disabled.
	*/
	uint16_t command;
	/*
	bit 15: Detected Parity Error - This bit will be set to 1 whenever the device detects a parity error, even if parity error handling is disabled.
	bit 14: Signalled System Error - This bit will be set to 1 whenever the device asserts SERR#.
	bit 13: Received Master Abort - This bit will be set to 1, by a master device, whenever its transaction (except for Special Cycle transactions) is terminated with Master-Abort.
	bit 12: Received Target Abort - This bit will be set to 1, by a master device, whenever its transaction is terminated with Target-Abort.
	bit 11: Signalled Target Abort - This bit will be set to 1 whenever a target device terminates a transaction with Target-Abort.
	bit 10 and 9: DEVSEL Timing - Read only bits that represent the slowest time that a device will assert DEVSEL# for any bus command except Configuration Space read and writes. Where a value of 0x00 represents fast timing, a value of 0x01 represents medium timing, and a value of 0x02 represents slow timing.
	bit 8: Master Data Parity Error - This bit is only set when the following conditions are met. The bus agent asserted PERR# on a read or observed an assertion of PERR# on a write, the agent setting the bit acted as the bus master for the operation in which the error occurred, and bit 6 of the Command register (Parity Error Response bit) is set to 1.
	bit 7: Fast Back-to-Back Capable - If set to 1 the device can accept fast back-to-back transactions that are not from the same agent; otherwise, transactions can only be accepted from the same agent.
	bit 6: reserved
	bit 5: 66 MHz Capable - If set to 1 the device is capable of running at 66 MHz; otherwise, the device runs at 33 MHz.
	bit 4: Capabilities List - If set to 1 the device implements the pointer for a New Capabilities Linked list at offset 0x34; otherwise, the linked list is not available.
	bit 3: Interrupt Status - Represents the state of the device's INTx# signal. If set to 1 and bit 10 of the Command register (Interrupt Disable bit) is set to 0 the signal will be asserted; otherwise, the signal will be ignored.
	bit 2 to 0: reserved
	*/
	uint16_t status;
	/*
	Class code, subclass and Prog IF are used to identify the device type, function and the device's register-level programming interface, respectively.
	See https://pci-ids.ucw.cz/read/PD/ for a list
	*/
	uint8_t revision_id;
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type; /* bit 7 = multiple function or not. bit 6 to 0 = header type */
	uint8_t bist; /* BIST = Build In Self Test. bit 7 = supports BIST. bit 6 = start BIST (if doesn't start after 2s then failure). bit 5 and 4 = reserved. bit 3 to 0: completion code, 0 after BIST execution if everything ok */
 	uint32_t dw0;
	uint32_t dw1;
	uint32_t dw2;
	uint32_t dw3;
	uint32_t dw4;
	uint32_t dw5;
	uint32_t dw6;
	uint32_t dw7;
	uint32_t dw8;
	uint32_t dw9;
	uint32_t dw10;
	uint32_t dw11;
	uint32_t dw12;
	uint32_t dw13;
}__attribute__((packed)) generic_pci_device_header_t;

typedef struct type00_pci_device_header {
 
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;
 	uint32_t bar0;
	uint32_t bar1;
	uint32_t bar2;
	uint32_t bar3;
	uint32_t bar4;
	uint32_t bar5;
	uint32_t cardbus_cis_pointer6; /* Points to the Card Information Structure and is used by devices that share silicon between CardBus and PCI */
	uint16_t subsystem_vendor_id;
	uint16_t subsystem_id;
	uint32_t expansion_rom_base_addr;
	uint8_t capabilities_pointer; /* Points (i.e. an offset into this function's configuration space) to a linked list of new capabilities implemented by the device. Used if bit 4 of the status register (Capabilities List bit) is set to 1. The bottom two bits are reserved and should be masked before the Pointer is used to access the Configuration Space. */
	uint8_t reserved2;
	uint16_t reserved1;
	uint32_t reserved3;
	uint8_t interrupt_line; /* PIC IRQ numbers 0-15 (not APIC's one). 0xFF = no connection */
	uint8_t interrupt_pin; /* Specifies which interrupt pin the device uses. Where a value of 0x01 is INTA#, 0x02 is INTB#, 0x03 is INTC#, 0x04 is INTD#, and 0x00 means the device does not use an interrupt pin. */
	uint8_t min_grant; /* A read-only register that specifies the burst period length, in 1/4 microsecond units, that the device needs (assuming a 33 MHz clock rate). */
	uint8_t max_latency; /* A read-only register that specifies how often the device needs access to the PCI bus (in 1/4 microsecond units). */
	uint32_t unused1;
	uint32_t unused2;
}__attribute__((packed)) type00_pci_device_header_t;

/* PCI to PCI bridge */
typedef struct type01_pci_device_header {
 
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;
	/*
	Base Address Registers (or BARs) can be used to hold memory addresses used by the device, or offsets for port addresses. Typically, memory address BARs need to be located in physical ram while I/O space BARs can reside at any memory address (even beyond physical memory).
	BAR:
		bit 31 to 4: 16-Byte Aligned Base Address
		bit 3: Prefetchable
		bit 2 and 1: Type
			0x00 then the base register is 32-bits wide and can be mapped anywhere in the 32-bit Memory Space
			0x01 is reserved as of revision 3.0 of the PCI Local Bus Specification
			0x02 means the base register is 64-bits wide and can be mapped anywhere in the 64-bit Memory Space
		bit 0: always 0

		To calculate actual base address (32 bits): (BAR[x] & 0xFFFFFFF0)
		To determine the amount of address space needed by a PCI device, you must save the original value of the BAR, write a value of all 1's to the register, then read it back. The amount of memory can then be determined by masking the information bits, performing a bitwise NOT ('~' in C), and incrementing the value by 1. The original value of the BAR should then be restored. The BAR register is naturally aligned and as such you can only modify the bits that are set. For example, if a device utilizes 16 MB it will have BAR0 filled with 0xFF000000 (0x01000000 after decoding) and you can only modify the upper 8-bits.
		When a base address register is marked as Prefetchable, it means that the region does not have read side effects (reading from that memory range doesn't change any state), and it is allowed for the CPU to cache loads from that memory region and read it in bursts (typically cache line sized). Hardware is also allowed to merge repeated stores to the same address into one store of the latest value. If you are using paging and want maximum performance, you should map prefetchable MMIO regions as WT (write-through) instead of UC (uncacheable). On x86, frame buffers are the exception, they should be almost always be mapped WC (write-combining). 
	IO memory space:
		bit 31 to 2: 4-Byte Aligned Base Address
		bit 1: reserved
		bit 0: always 1
	*/
 	uint32_t bar0;
	uint32_t bar1;
	uint8_t primary_bus_number;
	uint8_t secondary_bus_number;
	uint8_t subordinate_bus_number;
	uint8_t secondary_latency_timer;
	uint8_t io_base;
	uint8_t io_limit;
	uint16_t secondary_status;
	uint16_t memory_base;
	uint16_t memory_limit;
	uint16_t prefetch_memory_base;
	uint16_t prefetch_memory_limit;
	uint32_t prefetch_limit_upper32;
	uint32_t prefetch_base_upper32;
	uint16_t io_base_upper16;
	uint16_t io_limit_upper16;
	uint8_t capabilities_pointer;
	uint8_t reserved2;
	uint16_t reserved1;
	uint32_t expansion_rom_base_addr;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint16_t bridge_control;
	uint32_t unused1;
	uint32_t unused2;
}__attribute__((packed)) type01_pci_device_header_t;

/* PCI-to-CardBus bridge */
typedef struct type02_pci_device_header {
 
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;
 	uint32_t cardbus_socket_base_addr;
	uint8_t capabilities_list_offset;
	uint8_t reserved1;
	uint16_t secondary_status;
	uint8_t pci_bus_number;
	uint8_t cardbus_bus_number;
	uint8_t subordinate_bus_number;
	uint8_t cardbus_latency_timer;
	uint32_t memory_base_addr0;
	uint32_t memory_limit0;
	uint32_t memory_base_addr1;
	uint32_t memory_limit1;
	uint32_t io_base_addr0;
	uint32_t io_limit0;
	uint32_t io_base_addr1;
	uint32_t io_limit1;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint16_t bridge_control;
	uint16_t subsystem_vendor_id;
	uint16_t subsystem_id;
	uint32_t bit16_PC_legacy_base_addr;
}__attribute__((packed)) type02_pci_device_header_t;

void scan_and_register_pci_devices();

type00_pci_device_header_t* get_ahci_disk();

generic_pci_device_header_t* get_device_list();

int get_device_count();

generic_pci_device_header_t* device_list;
int device_count;

#endif