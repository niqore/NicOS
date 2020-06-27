#ifndef STDLIB_H
#define STDLIB_H

void init_memory_allocator();

void * memcpy(void *dest, const void *src, int len);

void* malloc(unsigned int size);

void free(void* ptr);

void * memset (void * dest, int val, unsigned int len);

void * calloc(unsigned int size);

void * realloc(void * ptr, unsigned int size);

#endif