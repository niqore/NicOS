#ifndef LIBC_H
#define LIBC_H

#include "../../filesystem/filesystem.h"
#include "../../filesystem/fat32.h"
#include "../../cpu/types.h"

typedef struct _libc {
	void (*printf)(const char *format, ...);

	FILE_PATH* (*filename_to_path)(char* dir, FILE_PATH* base_path);
	FILE_ENTRY* (*get_file_entry)(FILE_PATH* path);
	FILE_ENTRY* (*get_dir_entries)(int sector);
	uint32_t (*get_next_sector)(int sector);
	FILE_PATH* (*combine_paths)(FILE_PATH* path1, FILE_PATH* path2);
	void (*free_path)(FILE_PATH* path);

	void* (*malloc)(unsigned int size);
	void (*free)(void* ptr);

	char (*lower_case)(char c);
} LIBC;

#endif