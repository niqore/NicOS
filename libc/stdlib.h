#ifndef STDLIB_H
#define STDLIB_H

void init_memory_allocator();

void * memcpy(void *dest, const void *src, int len);

void* malloc(unsigned int size);

#endif