#include "fat32.h"
#include "../drivers/ahci.h"
#include "../libc/stdio.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"
#include "filesystem.h"

void init_fat32_filesystem() {

	uint16_t* buff = (uint16_t*) malloc(sizeof(uint16_t)*512/2);
	int res = ahci_read(0, 0, 0, 1, buff);
	if (!res) {
		printf("Error while reading disk. Halting...");
		__asm__ __volatile__("hlt");
	}
	filesystem_header = (struct fat32_header*) malloc(sizeof(struct fat32_header));

	//Il faut tout redefinir car la mémoire n'est pas alignée
	unsigned char* tmp = (unsigned char*) buff;
	memcpy(filesystem_header->oem_name, tmp + 0x03, 8);
	memcpy(&filesystem_header->bytes_per_sector, tmp + 0x0B, sizeof(uint16_t));
	memcpy(&filesystem_header->sectors_per_cluster, tmp + 0x0D, sizeof(uint8_t));
	memcpy(&filesystem_header->reserved_sectors, tmp + 0x0E, sizeof(uint16_t));
	memcpy(&filesystem_header->fat_number, tmp + 0x10, sizeof(uint8_t));
	memcpy(&filesystem_header->root_dir_entries, tmp + 0x11, sizeof(uint16_t));
	memcpy(&filesystem_header->logical_sectors_total, tmp + 0x13, sizeof(uint16_t));
	memcpy(&filesystem_header->media_descriptor, tmp + 0x15, sizeof(uint8_t));
	memcpy(&filesystem_header->logical_sectors_per_fat, tmp + 0x16, sizeof(uint16_t));
	memcpy(&filesystem_header->sectors_per_track, tmp + 0x18, sizeof(uint16_t));
	memcpy(&filesystem_header->heads_number, tmp + 0x1A, sizeof(uint16_t));
	memcpy(&filesystem_header->hidden_sectors, tmp + 0x1C, sizeof(uint32_t));
	memcpy(&filesystem_header->large_sector_count, tmp + 0x20, sizeof(uint32_t));
	memcpy(&filesystem_header->sectors_per_fat, tmp + 0x24, sizeof(uint32_t));
	memcpy(&filesystem_header->flags, tmp + 0x28, sizeof(uint16_t));
	memcpy(&filesystem_header->fat_version, tmp + 0x2A, sizeof(uint16_t));
	memcpy(&filesystem_header->root_dir_cluster, tmp + 0x2C, sizeof(uint32_t));
	memcpy(&filesystem_header->fs_info_sector, tmp + 0x30, sizeof(uint16_t));
	memcpy(&filesystem_header->backup_boot_sector, tmp + 0x32, sizeof(uint16_t));
	memcpy(filesystem_header->reserved, tmp + 0x34, 12);
	memcpy(&filesystem_header->drive_number, tmp + 0x40, sizeof(uint8_t));
	memcpy(&filesystem_header->windows_nt_flag, tmp + 0x41, sizeof(uint8_t));
	memcpy(&filesystem_header->signature, tmp + 0x42, sizeof(uint8_t));
	memcpy(&filesystem_header->volume_id_serial, tmp + 0x43, sizeof(uint32_t));
	memcpy(filesystem_header->volume_label, tmp + 0x47, 11);
	memcpy(filesystem_header->system_identifier, tmp + 0x52, 11);

	if (filesystem_header->signature != 0x28 && filesystem_header->signature != 0x29) {
		printf("Invalid FAT signature. Halting...");
		__asm__ __volatile__("hlt");
	}
	if (filesystem_header->system_identifier[0] != 'F' || filesystem_header->system_identifier[1] != 'A' || filesystem_header->system_identifier[2] != 'T'
		|| filesystem_header->system_identifier[3] != '3' || filesystem_header->system_identifier[4] != '2' || filesystem_header->system_identifier[5] != ' '
		|| filesystem_header->system_identifier[6] != ' ' || filesystem_header->system_identifier[7] != ' ') {
		printf("Invalid FAT system identifier. Halting...");
		__asm__ __volatile__("hlt");
	}

	free(buff);
}