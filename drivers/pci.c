#include "pci.h"
#include "../cpu/ports.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/stdio.h"

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

void scan_and_register_pci_devices(generic_pci_device_header_t* list) {

	uint8_t bus, device, func;
	uint32_t data;
	int index = 0;

	for(bus = 0; bus != 0xff; bus++) {
		for(device = 0; device < 32; device++) {
			for(func = 0; func < 8; func++) {
				data = r_pci_32(bus, device, func, 0);
				if(data != 0xffffffff) {
					for (int i = 0; i < 18; ++i) {
						((uint32_t*) &list[index])[i] = r_pci_32(bus, device, func, i);
					}
					index++;
				}
			}
		}
	}
}

void print_pci_device_info(generic_pci_device_header_t* device) {
	printf("Device ID: 0x%x; Vendor ID: 0x%x; Class code: 0x%x; Subclass: 0x%x; Prof IF: 0x%x; Revision ID: 0x%x", device->device_id, device->vendor_id, device->class_code, device->subclass, device->prog_if, device->revision_id);
}

void print_pci_devices_info(generic_pci_device_header_t* list, int count) {
	for (int i = 0; i < count; ++i) {
		print_pci_device_info(&list[i]);
		printf("\n\n");
	}
}