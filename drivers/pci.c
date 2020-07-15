#include "pci.h"
#include "../cpu/ports.h"
#include "screen.h"
#include "../libc/string.h"
#include "../cpu/types.h"

const uint32_t PCI_ENABLE_BIT     = 0x80000000;
const uint32_t PCI_CONFIG_ADDRESS = 0xCF8;
const uint32_t PCI_CONFIG_DATA    = 0xCFC;

uint32_t r_pci_32(uint8_t bus, uint8_t device, uint8_t func, uint8_t pcireg) {

	outl(PCI_ENABLE_BIT | (bus << 16) | (device << 11) | (func << 8) | (pcireg << 2), PCI_CONFIG_ADDRESS);
	uint32_t ret = inl(PCI_CONFIG_DATA);

	return ret;
}

void scan_and_register_devices() {

	uint8_t bus, device, func;
	uint32_t data;
	char buffer[8];

	for(bus = 0; bus != 0xff; bus++) {
		for(device = 0; device < 32; device++) {
			for(func = 0; func < 8; func++) {
				data = r_pci_32(bus, device, func, 0);
				if(data != 0xffffffff) {
					print_string("Bus: ");
					print_string(itoa(bus, buffer, 10));
					print_string(" | Device: ");
					print_string(itoa(device, buffer, 10));
					print_string(" | Function: ");
					print_string(itoa(func, buffer, 10));
					print_string(" | VendorID: 0x");
					print_string(itoa(data, buffer, 16));
					print_string("\n");
				}
			}
		}
	}
}