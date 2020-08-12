#ifndef LIBC_H
#define LIBC_H

#include "../../filesystem/filesystem.h"
#include "../../filesystem/fat32.h"
#include "../../cpu/types.h"
#include "../../drivers/pci.h"
#include "../mem.h"

typedef struct _libc {
	void (*printf)(const char *format, ...);

	FILE_PATH* (*filename_to_path)(char* dir, FILE_PATH* base_path);
	FILE_ENTRY* (*get_file_entry)(FILE_PATH* path);
	FILE_ENTRY* (*get_dir_entries)(int sector);
	uint32_t (*get_next_sector)(int sector);
	FILE_PATH* (*combine_paths)(FILE_PATH* path1, FILE_PATH* path2);
	void (*free_path)(FILE_PATH* path);
	unsigned char* (*read_fat32_file)(FILE_ENTRY* file);

	void* (*malloc)(unsigned int size);
	void (*free)(void* ptr);

	void (*strrev)(unsigned char *str);
	char* (*itoa)(int value, char* buffer, int base);
	int (*strlen)(const char *str);
	char (*lower_case)(char c);
	char (*upper_case)(char c);
	int (*strcmp)(const char *p1, const char *p2);
	char* (*format_number_decimals)(char *s, int decimals);

	uint8_t (*get_last_key)();
	void (*reset_last_key)();

	int (*print_char_at)(char c, int col, int row, char attribute);
	void (*clear_screen)();

	uint32_t (*rand)(void);
	void (*srand)(uint32_t seed);
	uint32_t (*random_seed)();
	uint32_t (*randint)(uint32_t min, uint32_t max);

	void (*sleep)(uint32_t ms);

	generic_pci_device_header_t* (*get_device_list)();
	int (*get_device_count)();

	multiboot_memory_map_t* (*get_memory_map)();
	int (*get_memory_map_size)();
} LIBC;

#endif