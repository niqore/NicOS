#include "fat32.h"
#include "../drivers/ahci.h"
#include "../libc/stdio.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"

void init_fat32_filesystem() {

	uint16_t* buff = (uint16_t*) malloc(sizeof(uint16_t)*512/2);
	int res = ahci_read(0, 0, 0, 1, buff);
	if (!res) {
		printf("Error while reading disk. Halting...");
		__asm__ __volatile__("hlt");
	}
	filesystem_header = (struct fat32_header*) ((char*) buff + 3);

	if (filesystem_header->bytes_per_sector != 512) {
		printf("This system doesn't support FAT with bytes per sectors not equals to 512. Halting...");
		__asm__ __volatile__("hlt");
	}
	if (filesystem_header->sectors_per_cluster != 1) {
		printf("This system doesn't support FAT sectors per cluster not equals to 1. Halting...");
		__asm__ __volatile__("hlt");
	}
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
}

uint32_t get_next_sector(int sector) {
	uint32_t fat_addr = filesystem_header->reserved_sectors * 512 + sector * 4;
	uint16_t* buff = (uint16_t*) malloc(512);
	int res = ahci_read(0, fat_addr / 512, 0, 1, buff);
	if (!res) {
		printf("Error while reading disk. Halting...");
		__asm__ __volatile__("hlt");
	}
	uint32_t next_sector = *((uint32_t*) ((char*) buff + fat_addr % 512));
	free(buff);
	return next_sector & 0x00ffffff;
}

int get_sector_shifted(int sector) {
	return filesystem_header->reserved_sectors + filesystem_header->fat_number * filesystem_header->sectors_per_fat + sector - 2;
}

int get_root_sector() {
	return filesystem_header->root_dir_cluster;
}

int is_vfat_entry(FILE_ENTRY* entry) {
	return entry->attribute == 0xF;
}

FILE_ENTRY* get_dir_entries(int sector) {
	uint16_t* buff = (uint16_t*) malloc(filesystem_header->bytes_per_sector);
	int res = ahci_read(0, get_sector_shifted(sector), 0, 1, buff);
	if (!res) {
		printf("Error while reading disk. Halting...");
		__asm__ __volatile__("hlt");
	}
	return (FILE_ENTRY*) buff;
}

FILE_ENTRY* get_root_dir_entries() {
	return get_dir_entries(get_root_sector());
}

FILE_ENTRY* get_file_entry(FILE_PATH* path) {

	if (path == 0) {
		FILE_ENTRY* fentry = (FILE_ENTRY*) malloc(sizeof(FILE_ENTRY));
		memset(fentry->short_name, ' ', 11);
		fentry->attribute = FAT32_ATTR_SUBDIR;
		fentry->ext_attributes = 0;
		fentry->create_time_ms = 0;
		fentry->create_date = 0;
		fentry->last_access_date = 0;
		fentry->cluster_high_bytes = 0;
		fentry->modified_time = 0;
		fentry->modified_date = 0;
		fentry->cluster_low_bytes = filesystem_header->root_dir_cluster;
		fentry->file_size = 0;
		return fentry;
	}
	FILE_ENTRY* dir = get_root_dir_entries();
	uint32_t curr_sector = get_root_sector();
	FILE_PATH* cur_path = path;
	int skip_vfat = 0;
	int in_vfat = 0;
	int i = 0;
	while (dir != 0 && cur_path != 0) {
		int name_length = strlen(cur_path->name);
		int extension_length = 0;
		int j;
		for (j = name_length - 1; j != name_length - 5; --j) {
			if (cur_path->name[j] == '.') {
				extension_length = name_length - 1 - j;
				break;
			}
		}
		for (i = 0; i < 16; ++i) {
			if (is_vfat_entry(dir + i)) {
				if (skip_vfat) {
					continue;
				}
				VFAT_ENTRY* vfat_ent = (VFAT_ENTRY*) &dir[i];
				in_vfat = 1;
				int seq_num = vfat_ent->sequence_number & 0x1F;
				if (name_length + 13 < seq_num * 13) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 0 && lower_case(vfat_ent->char1) != lower_case(cur_path->name[13 * (seq_num - 1) + 0])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 1 && lower_case(vfat_ent->char2) != lower_case(cur_path->name[13 * (seq_num - 1) + 1])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 2 && lower_case(vfat_ent->char3) != lower_case(cur_path->name[13 * (seq_num - 1) + 2])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 3 && lower_case(vfat_ent->char4) != lower_case(cur_path->name[13 * (seq_num - 1) + 3])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 4 && lower_case(vfat_ent->char5) != lower_case(cur_path->name[13 * (seq_num - 1) + 4])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 5 && lower_case(vfat_ent->char6) != lower_case(cur_path->name[13 * (seq_num - 1) + 5])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 6 && lower_case(vfat_ent->char7) != lower_case(cur_path->name[13 * (seq_num - 1) + 6])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 7 && lower_case(vfat_ent->char8) != lower_case(cur_path->name[13 * (seq_num - 1) + 7])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 8 && lower_case(vfat_ent->char9) != lower_case(cur_path->name[13 * (seq_num - 1) + 8])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 9 && lower_case(vfat_ent->char10) != lower_case(cur_path->name[13 * (seq_num - 1) + 9])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 10 && lower_case(vfat_ent->char11) != lower_case(cur_path->name[13 * (seq_num - 1) + 10])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 11 && lower_case(vfat_ent->char12) != lower_case(cur_path->name[13 * (seq_num - 1) + 11])) {
					goto skip_vfat_entry;
				}
				if (name_length + 1 > 13 * (seq_num - 1) + 12 && lower_case(vfat_ent->char13) != lower_case(cur_path->name[13 * (seq_num - 1) + 12])) {
					goto skip_vfat_entry;
				}
				continue;
				skip_vfat_entry:
					skip_vfat = 1;
					continue;
			}
			else {
				if (skip_vfat) { // L'entrée vfat est invalide, on passe à l'entrée suivante
					in_vfat = 0;
					skip_vfat = 0;
					continue;
				}
				else if (in_vfat) {
					in_vfat = 0;
					break; // On a validé le nom vfat
				}
				else {
					in_vfat = 0;
					if (extension_length != 0) {
						for (j = name_length - extension_length; j < name_length; ++j) {
							if (lower_case(cur_path->name[j]) != lower_case(dir[i].extension[j - (name_length - extension_length)])) {
								break;
							}
						}
						if (j != name_length) {
							continue;
						}
					}
					for (j = 0; j < name_length - (extension_length != 0 ? extension_length + 1 : 0); ++j) {
						if (lower_case(cur_path->name[j]) != lower_case(dir[i].short_name[j])) {
							break;
						}
					}
					if (j != name_length - (extension_length != 0 ? extension_length + 1 : 0)) {
						continue;
					}
					for (j = name_length - (extension_length != 0 ? extension_length + 1 : 0); j < 8; ++j) {
						if (dir[i].short_name[j] != ' ') {
							break;
						}
					}
					if (j != 8) {
						continue;
					}
					break; // Le nom dans l'entrée est validé
				}
			}
		}
		if (i != 16) {
			if (cur_path->next == 0) {
				break;
			}
			FILE_ENTRY* tmp_dir = dir;
			curr_sector = ((uint32_t) dir[i].cluster_high_bytes << 16) + dir[i].cluster_low_bytes;
			dir = get_dir_entries(curr_sector);
			free(tmp_dir);
			cur_path = cur_path->next;
		}
		else {
			uint32_t next_sect = get_next_sector(curr_sector);
			if (next_sect >= 0xffffef) {
				free(dir);
				dir = 0;
			}
			else {
				FILE_ENTRY* tmp_dir = dir;
				dir = get_dir_entries(next_sect);
				curr_sector = next_sect;
				free(tmp_dir);
			}
		}
	}
	FILE_ENTRY* ret = 0;
	if (dir != 0) {
		ret = (FILE_ENTRY*) malloc(sizeof(FILE_ENTRY));
		memcpy(ret, dir + i, sizeof(FILE_ENTRY));
		free(dir);
	}
	return ret;
}

unsigned char* read_fat32_file(FILE_ENTRY* file) {
	if (file->file_size == 0) return 0;
	uint16_t* ptr = (uint16_t*) malloc(((file->file_size / 512) + 1) * 512); // On arrondi au-dessus car en ahci on ne peut lire que 512 octets à la fois minimum
	uint16_t* ptrPos = ptr;
	uint32_t cur_sector = ((uint32_t) file->cluster_high_bytes << 16) + file->cluster_low_bytes;
	while (cur_sector < 0xffffef) {
		int res = ahci_read(0, get_sector_shifted(cur_sector), 0, 1, ptrPos);
		if (!res) {
			printf("Error while reading disk. Halting...");
			__asm__ __volatile__("hlt");
		}
		ptrPos += 256; // 512 octets vu que c'est du uint16_t
		cur_sector = get_next_sector(cur_sector);
	}
	return (unsigned char*) ptr;
}