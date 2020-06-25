#include "mem.h"
#include "stdlib.h"

void * memcpy(void *dest, const void *src, int len) {

	char *d = dest;
	const char *s = src;
	while (len--) {
		*d++ = *s++;
	}
	return dest;
}

unsigned int real_free_memory(unsigned int size) {
	return size - sizeof(allocated_memory_block);
}

void init_memory_allocator() {
	free_list = (void*) FIRST_FREE_ADDR;
	free_list->size = real_free_memory(MAX_FREE_ADDR - FIRST_FREE_ADDR);
	free_list->next = free_list;
}

void* malloc(unsigned int size) {

	
}