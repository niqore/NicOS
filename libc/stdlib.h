#ifndef STDLIB_H
#define STDLIB_H

#include "../kernel/multiboot2.h"

void init_memory_allocator(struct multiboot_tag *map);

void * memcpy(void *dest, const void *src, int len);

void* malloc(unsigned int size);

void free(void* ptr);

void * memset (void * dest, int val, unsigned int len);

void * calloc(unsigned int size);

void * realloc(void * ptr, unsigned int size);

#endif