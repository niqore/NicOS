#ifndef AHCI_H
#define AHCI_H

#include "../cpu/types.h"
#include "pci.h"

typedef type00_pci_device_header_t ahci_pci_header;

struct generic_host_control {
	uint32_t host_capabilities;
	uint32_t global_host_control;
	uint32_t interrupt_status;
	uint32_t ports_implemented;
	uint32_t version;
	uint32_t ccc_ctl; //Command Completion Coalescing Control
	uint32_t ccc_ports; //Command Completion Coalsecing Ports
	uint32_t em_loc; // Enclosure Management Location
	uint32_t em_ctl; // Enclosure Management Control
	uint32_t host_cap_extended;
	uint32_t bios_handoff;
};

struct ahci_port_register {
	uint32_t cl_base_addr; //Command List Base Address
	uint32_t cl_base_addr_upper;
	uint32_t fis_base_addr;
	uint32_t fis_base_addr_upper;
	uint32_t interrupt_status;
	uint32_t interrupt_enable;
	uint32_t cmd_status;
	uint32_t reserved1;
	uint32_t task_file_data;
	uint32_t signature;
	uint32_t sata_status;
	uint32_t sata_control;
	uint32_t sata_error;
	uint32_t sata_active;
	uint32_t command_issue;
	uint32_t sata_notif;
	uint32_t fis_switch_ctl;
	uint32_t device_sleep;
	uint32_t reserved2;
	uint32_t reserved3;
	uint32_t reserved4;
	uint32_t reserved5;
	uint32_t reserved6;
	uint32_t reserved7;
	uint32_t reserved8;
	uint32_t reserved9;
	uint32_t reserved10;
	uint32_t reserved11;
	uint32_t vendor_specific1;
	uint32_t vendor_specific2;
	uint32_t vendor_specific3;
	uint32_t vendor_specific4;
};

struct hba_memory_register {
	struct generic_host_control host_control;
	uint32_t reserved[13];
	uint32_t reserved_nvmhci[16];
	uint32_t vendor_specific_regs[24];
	struct ahci_port_register ports[32];
};

struct ahci_command_list {

	uint8_t  cfl:5;		// Command FIS length in DWORDS, 2 ~ 16
	uint8_t  a:1;		// ATAPI
	uint8_t  w:1;		// Write, 1: H2D, 0: D2H
	uint8_t  p:1;		// Prefetchable
 
	uint8_t  r:1;		// Reset
	uint8_t  b:1;		// BIST
	uint8_t  c:1;		// Clear busy upon R_OK
	uint8_t  rsv0:1;		// Reserved
	uint8_t  pmp:4;		// Port multiplier port
 
	uint16_t prdtl;		// Physical region descriptor table length in entries
 
	volatile
	uint32_t prdbc;		// Physical region descriptor byte count transferred
 
	uint32_t ctba;		// Command table descriptor base address
	uint32_t ctbau;		// Command table descriptor base address upper 32 bits

	uint32_t rsv1[4];	// Reserved
};

struct fis_reg_h2d {

	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_REG_H2D
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:3;		// Reserved
	uint8_t  c:1;		// 1: Command, 0: Control
 
	uint8_t  command;	// Command register
	uint8_t  featurel;	// Feature register, 7:0
 
	// DWORD 1
	uint8_t  lba0;		// LBA low register, 7:0
	uint8_t  lba1;		// LBA mid register, 15:8
	uint8_t  lba2;		// LBA high register, 23:16
	uint8_t  device;		// Device register
 
	// DWORD 2
	uint8_t  lba3;		// LBA register, 31:24
	uint8_t  lba4;		// LBA register, 39:32
	uint8_t  lba5;		// LBA register, 47:40
	uint8_t  featureh;	// Feature register, 15:8
 
	// DWORD 3
	uint8_t  countl;		// Count register, 7:0
	uint8_t  counth;		// Count register, 15:8
	uint8_t  icc;		// Isochronous command completion
	uint8_t  control;	// Control register
 
	// DWORD 4
	uint8_t  rsv1[4];	// Reserved
};

struct ahci_prdt_entry {
	uint32_t dba;		// Data base address
	uint32_t dbau;		// Data base address upper 32 bits
	uint32_t rsv0;		// Reserved
 
	// DW3
	uint32_t dbc:22;		// Byte count, 4M max
	uint32_t rsv1:9;		// Reserved
	uint32_t i:1;		// Interrupt on completion
};

struct ahci_cmd_tbl {
	// 0x00
	uint8_t  cfis[64];	// Command FIS
 
	// 0x40
	uint8_t  acmd[16];	// ATAPI command, 12 or 16 bytes
 
	// 0x50
	uint8_t  rsv[48];	// Reserved
 
	// 0x80
	struct ahci_prdt_entry prdt_entry[1];	// Physical region descriptor table entries, 0 ~ 65535
} HBA_CMD_TBL;

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08
// Register FIS - host to device
#define FIS_TYPE_REG_H2D 0x27
// Register FIS - device to host
#define FIS_TYPE_REG_D2H 0x34
// DMA activate FIS - device to host
#define FIS_TYPE_DMA_ACT 0x39
// DMA setup FIS - bidirectional
#define FIS_TYPE_DMA_SETUP 0x41
// Data FIS - bidirectional
#define FIS_TYPE_DATA 0x46
// BIST activate FIS - bidirectional
#define FIS_TYPE_BIST 0x58
// PIO setup FIS - device to host
#define FIS_TYPE_PIO_SETUP 0x5F
// Set device bits FIS - device to host
#define FIS_TYPE_DEV_BITS 0xA1
#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EX 0x35
#define HBA_PxIS_TFES (1 << 30)

void register_ahci_disk(ahci_pci_header* pci);

int ahci_read(int port_num, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf);

ahci_pci_header* ahci_pci_device;
struct hba_memory_register* ahci_hba;

#endif