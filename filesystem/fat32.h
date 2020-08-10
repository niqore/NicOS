#ifndef FAT32_H
#define FAT32_H

#include "../cpu/types.h"
#include "filesystem.h"

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
	uint32_t root_dir_cluster; // Souvent 2
	uint16_t fs_info_sector;
	uint16_t backup_boot_sector;
	uint8_t reserved[12];
	uint8_t drive_number;
	uint8_t windows_nt_flag;
	uint8_t signature; // Doit être 0x28 ou 0x29
	uint32_t volume_id_serial;
	char volume_label[11];
	char system_identifier[8]; // Toujours "FAT32   "
}__attribute__((packed));

typedef struct fat32_file_entry {

	char short_name[8];
	char extension[3];
	uint8_t attribute;
	uint8_t ext_attributes;
	uint8_t create_time_ms; // unité de 10 ms
	uint16_t create_time; // bits 15-11: heure (0-23), bits 10-5: minutes (0-59), bits 4-0: seconds/2 (0-29)
	uint16_t create_date; // bits 15-9: année (0 = 1980, 119 = 2099), 8-5: mois (1-12), 4-0: jour (1-31)
	uint16_t last_access_date;
	uint16_t cluster_high_bytes;
	uint16_t modified_time; // bits 15-11: heure (0-23), bits 10-5: minutes (0-59), bits 4-0: seconds/2 (0-29)
	uint16_t modified_date; // bits 15-9: année (0 = 1980, 119 = 2099), 8-5: mois (1-12), 4-0: jour (1-31)
	uint16_t cluster_low_bytes;
	uint32_t file_size;
}__attribute__((packed)) FILE_ENTRY;

typedef struct vfat_entry {
	/*
	Bit 7: 0
	Bit 6: Dernier de la séquence
	Bit 5: 0
	Bit 4-0: Numéro de séquence

	Note: 0xE5 = supprimé
	*/
	char sequence_number;
	// Si le caractère est 0xFF, c'est la fin. Le dernier caractère est 0x00 puis il n'y a que des 0xFF
	char char1;
	char dump2;
	char char2;
	char dump3;
	char char3;
	char dump4;
	char char4;
	char dump5;
	char char5;
	char dump6;
	char attribute;
	char type; // Toujours 0x00
	/* 
	Algorithme du checksum:

	int sum = 0;
   	for (int i = 0; i < length of complete filename; ++i)
    	sum = ((sum & 1) << 7) + (sum >> 1) + filename[i];
    return sum;
	*/
	char name_checksum;
	char char6;
	char dump7;
	char char7;
	char dump8;
	char char8;
	char dump9;
	char char9;
	char dump10;
	char char10;
	char dump11;
	char char11;
	char dump12;
	uint16_t first_dos_cluster; // Toujours 0x0000
	char char12;
	char dump13;
	char char13;
	char dump14;

}__attribute__((packed)) VFAT_ENTRY;

#define FAT32_ATTR_READ_ONLY 0x01
#define FAT32_ATTR_HIDDEN 0x02
#define FAT32_ATTR_SYSTEM 0x04
#define FAT32_ATTR_VOLUME_LABEL 0x08
#define FAT32_ATTR_SUBDIR 0x10
#define FAT32_ATTR_ARCHIVE 0x20
#define FAT32_ATTR_DEVICE 0x40

struct fat32_header * filesystem_header;

void init_fat32_filesystem();

FILE_ENTRY* get_file_entry(FILE_PATH* path);

unsigned char* read_fat32_file(FILE_ENTRY* file);

FILE_ENTRY* get_dir_entries(int sector);

uint32_t get_next_sector(int sector);

#endif