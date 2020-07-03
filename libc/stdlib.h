#ifndef STDLIB_H
#define STDLIB_H

#include "../kernel/memory_map.h"

void init_memory_allocator(SMAP_entry_t* map, int entries);

void * memcpy(void *dest, const void *src, int len);

void* malloc(unsigned int size);

void free(void* ptr);

void * memset (void * dest, int val, unsigned int len);

void * calloc(unsigned int size);

void * realloc(void * ptr, unsigned int size);

void print_ram_info(SMAP_entry_t* map, int entries);

#endif