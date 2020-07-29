#include "ahci.h"
#include "../libc/stdio.h"
#include "../libc/stdlib.h"

void register_ahci_disk(ahci_pci_header* pci) {
	ahci_pci_device = pci;
	ahci_hba = (struct hba_memory_register*) pci->bar5;
}

// Find a free command list slot
int find_ahci_cmdslot(struct ahci_port_register *port) {

	// If not set in SACT and CI, the slot is free
	uint32_t slots = (port->sata_active | port->command_issue);
	for (int i = 0; i < 32; i++) {
		if ((slots & 1) == 0)
			return i;
		slots >>= 1;
	}
	printf("Cannot find free command list entry\n");
	return -1;
}

int ahci_read(int port_num, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf) {

	if (port_num > 31 || port_num < 0) {
		printf("Incorrect port number\n");
		return 0;
	}
	struct ahci_port_register* port = &ahci_hba->ports[port_num];

	port->interrupt_status = -1; // Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_ahci_cmdslot(port);
	if (slot == -1)
		return 0;
 
	struct ahci_command_list* cmd_header = (struct ahci_command_list*) port->cl_base_addr;
	cmd_header += slot;
	cmd_header->cfl = sizeof(struct fis_reg_h2d) / sizeof(uint32_t); // Command FIS size
	cmd_header->w = 0; // Read from device
	cmd_header->prdtl = (uint16_t) ((count - 1) >> 4) + 1; // PRDT entries count
 
	struct ahci_cmd_tbl *cmdtbl = (struct ahci_cmd_tbl*) (cmd_header->ctba);
	memset(cmdtbl, 0, sizeof(struct ahci_cmd_tbl) + (cmd_header->prdtl-1) * sizeof(struct ahci_prdt_entry));
 
	// 8K bytes (16 sectors) per PRDT
	int i = 0;
	for (i = 0; i < cmd_header->prdtl - 1; i++) {
		cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024-1; // 8K bytes (this value should always be set to 1 less than the actual value)
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
	cmdtbl->prdt_entry[i].dbc = (count << 9) - 1; // 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
 
	// Setup command
	struct fis_reg_h2d *cmdfis = (struct fis_reg_h2d*) (&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
 
	cmdfis->lba0 = (uint8_t) startl;
	cmdfis->lba1 = (uint8_t) (startl >> 8);
	cmdfis->lba2 = (uint8_t) (startl >> 16);
	cmdfis->device = 1 << 6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t) (startl >> 24);
	cmdfis->lba4 = (uint8_t) starth;
	cmdfis->lba5 = (uint8_t) (starth >> 8);
 
	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->task_file_data & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
		spin++;
	}
	if (spin == 1000000) {
		printf("Port is hung\n");
		return 0;
	}
 
	port->command_issue = 1 << slot;	// Issue command
 
	// Wait for completion
	while (1) {
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->command_issue & (1 << slot)) == 0) 
			break;
		if (port->interrupt_status & HBA_PxIS_TFES)	{ // Task file error
			printf("Read disk error\n");
			return 0;
		}
	}
 
	// Check again
	if (port->interrupt_status & HBA_PxIS_TFES) {
		printf("Read disk error\n");
		return 0;
	}
 
	return 1;
}
