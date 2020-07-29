#ifndef FAT32_H
#define FAT32_H

#include "../cpu/types.h"

struct fat32_header {

	//Commun aux FAT
	char oem_name[8];
	uint16_t bytes_per_sector; // Souvent 512
	uint8_t sectors_per_cluster; 
	uint16_t reserved_sectors; // Souvent 32
	uint8_t fat_number; // Souvent 2
	uint16_t root_dir_entries; // 0 pour le FAT32
	uint16_t logical_sectors_total; // 0 pour le FAT32
	uint8_t media_descriptor;
	uint16_t logical_sectors_per_fat; // 0 pour le FAT32
	uint16_t sectors_per_track;
	uint16_t heads_number;
	uint32_t hidden_sectors;
	uint32_t large_sector_count;

	//Exclusif au FAT32
	uint32_t sectors_per_fat;
	uint16_t flags;
	uint16_t fat_version;
	uint32_t root_dir_cluster; //Souvent 2
	uint16_t fs_info_sector;
	uint16_t backup_boot_sector;
	uint8_t reserved[12];
	uint8_t drive_number;
	uint8_t windows_nt_flag;
	uint8_t signature; //Doit être 0x28 ou 0x29
	uint32_t volume_id_serial;
	char volume_label[11];
	char system_identifier[8]; //Toujours "FAT32   "
};

struct fat32_header * filesystem_header;

void init_fat32_filesystem();

#endif