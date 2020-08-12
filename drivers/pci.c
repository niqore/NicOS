#include "pci.h"
#include "../cpu/ports.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../libc/stdlib.h"

const uint32_t PCI_ENABLE_BIT     = 0x80000000;
const uint32_t PCI_CONFIG_ADDRESS = 0xCF8;
const uint32_t PCI_CONFIG_DATA    = 0xCFC;

uint32_t r_pci_32(uint8_t bus, uint8_t device, uint8_t func, uint8_t pcireg) {

	outl(PCI_ENABLE_BIT | (bus << 16) | (device << 11) | (func << 8) | (pcireg << 2), PCI_CONFIG_ADDRESS);
	uint32_t ret = inl(PCI_CONFIG_DATA);

	return ret;
}

int count_pci_devices() {

	uint8_t bus, device, func;
	uint32_t data;
	int count = 0;

	for(bus = 0; bus != 0xff; bus++) {
		for(device = 0; device < 32; device++) {
			for(func = 0; func < 8; func++) {
				data = r_pci_32(bus, device, func, 0);
				if(data != 0xffffffff) {
					count++;
				}
			}
		}
	}
	return count;
}

void scan_and_register_pci_devices() {

	uint8_t bus, device, func;
	uint32_t data;
	int index = 0;

	device_count = count_pci_devices();
	device_list = (generic_pci_device_header_t*) malloc(device_count * sizeof(generic_pci_device_header_t));

	for(bus = 0; bus != 0xff; bus++) {
		for(device = 0; device < 32; device++) {
			for(func = 0; func < 8; func++) {
				data = r_pci_32(bus, device, func, 0);
				if(data != 0xffffffff) {
					for (int i = 0; i < 18; ++i) {
						((uint32_t*) &device_list[index])[i] = r_pci_32(bus, device, func, i);
					}
					index++;
				}
			}
		}
	}
}

type00_pci_device_header_t* get_ahci_disk() {
	for (int i = 0; i < device_count; ++i) {
		if (device_list[i].class_code == 0x01 && device_list[i].subclass == 0x06 && device_list[i].prog_if == 0x01) {
			return (type00_pci_device_header_t*) &device_list[i];
		}
	}
	return 0;
}

generic_pci_device_header_t* get_device_list() {
	return device_list;
}

int get_device_count() {
	return device_count;
}