#ifndef MEM_H
#define MEM_H

#include "../kernel/multiboot2.h"

typedef struct _allocated_memory_block allocated_memory_block;

struct _allocated_memory_block {
	unsigned int size;
};

typedef struct _free_memory_block free_memory_block;

struct _free_memory_block {
	unsigned int size;
	free_memory_block * next;
	free_memory_block * previous;
};

free_memory_block * free_list;
multiboot_memory_map_t * memory_map;
int mmap_size = 0;

#endif