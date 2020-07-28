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

char * get_device_description(generic_pci_device_header_t* device) {
	switch (device->class_code) {
		case 0x00:
			if (device->subclass == 0x00)
				return "Unclassified Non-VGA-Compatible devices";
			else if (device->subclass == 0x01)
				return "Unclassified VGA-Compatible Device";
			return "Unknown Unclassified Device";
		case 0x01:
			switch (device->subclass) {
				case 0x00:
					return "SCSI Bus Controller";
				case 0x01:
					return "IDE Controller";
				case 0x02:
					return "Floppy Disk Controller";
				case 0x03:
					return "IPI Bus Controller";
				case 0x04:
					return "RAID Controller";
				case 0x05:
					if (device->prog_if == 0x20)
						return "Single DMA ATA Controller";
					else if (device->prog_if == 0x30)
						return "Chained DMA ATA Controller";
					break;
				case 0x06:
					if (device->prog_if == 0x00)
						return "Vendor Specific SATA Interface";
					else if (device->prog_if == 0x01)
						return "AHCI 1.0";
					else if (device->prog_if == 0x02)
						return "Serial Storage Bus SATA";
					break;
				case 0x07:
					return "Serial Attached SCSI";
				case 0x08:
					return "Non-Volatile Memory Controller";
			}
			return "Unknown Mass Storage Controller";
		case 0x02:
			switch (device->subclass) {
				case 0x00:
					return "Ethernet Controller";
				case 0x01:
					return "Token Ring Controller";
				case 0x02:
					return "FDDI Controller";
				case 0x03:
					return "ATM Controller";
				case 0x04:
					return "ISDN Controller";
				case 0x05:
					return "WorldFip Controller";
				case 0x06:
					return "PICMG 2.14 Multi Computing";
				case 0x07:
					return "Infiniband Controller";
				case 0x08:
					return "Fabric Controller";
				default:
					return "Unknown Network Controller";
			}
		case 0x03:
			switch (device->subclass) {
				case 0x00:
					if (device->prog_if == 0x00)
						return "VGA Controller";
					else if (device->prog_if == 0x01)
						return "8514-Compatible VGA Controller";
					break;
				case 0x01:
					return "XGA Controller";
				case 0x02:
					return "3D Controller (Not VGA-Compatible)";
			}
			return "Unknown Display Controller";
		case 0x04:
			return "Multimedia Controller";
		case 0x05:
			return "Memory Controller";
		case 0x06:
			switch (device->subclass) {
				case 0x00:
					return "Host Bridge";
				case 0x01:
					return "ISA Bridge";
				case 0x02:
					return "EISA Bridge";
				case 0x03:
					return "MCA Bridge";
				case 0x04:
					return "PCI-to-PCI Bridge";
				case 0x05:
					return "PCMCIA Bridge";
				case 0x06:
					return "NuBus Bridge";
				case 0x07:
					return "CardBus Bridge";
				case 0x08:
					return "RACEway Bridge";
				case 0x09:
					if (device->prog_if == 0x40)
						return "PCI-to-PCI Bridge, Semi-Transparent, Primary bus towards host CPU";
					else if (device->prog_if == 0x80)
						return "PCI-to-PCI Bridge, Semi-Transparent, Secondary bus towards host CPU";
					return "Unknown PCI-to-PCI Bridge";
				case 0x0A:
					return "InfiniBand-to-PCI Host Bridge";
				default:
					return "Unknown Bridge Device";
			}
		case 0x07:
			return "Simple Communication Controller";
		case 0x08:
			return "Base System Peripheral";
		case 0x09:
			return "Input Device Controller";
		case 0x0A:
			return "Docking Station";
		case 0x0B:
			return "Processor";
		case 0x0C:
			return "Serial Bus Controller";
		case 0x0D:
			return "Wireless Controller";
		case 0x0E:
			return "Intelligent Controller";
		case 0x0F:
			return "Satellite Communication Controller";
		case 0x10:
			return "Encryption Controller";
		case 0x11:
			return "Signal Processing Controller";
		case 0x12:
			return "Processing Accelerator";
		case 0x13:
			return "Non-Essential Instrumentation";
		case 0x14:
			return "0x3F (Reserved)";
		case 0x40:
			return "Co-Processor";
		case 0x41:
			return "0xFE (Reserved)";
		case 0xFF:
			return "Unassigned Class (Vendor specific)";
	}
	return "Unknown Device";
}

void print_pci_device_info(generic_pci_device_header_t* device) {
	printf("DID: 0x%x; VID: 0x%x; Class: 0x%x; Subclass: 0x%x; Prof IF: 0x%x\n> %s", device->device_id, device->vendor_id, device->class_code, device->subclass, device->prog_if, get_device_description(device));
}

void print_pci_devices_info(generic_pci_device_header_t* list, int count) {
	for (int i = 0; i < count; ++i) {
		print_pci_device_info(&list[i]);
		printf("\n");
	}
}