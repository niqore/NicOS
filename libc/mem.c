#include "mem.h"
#include "stdlib.h"
#include "../drivers/screen.h"
#include "string.h"
#include "stdio.h"
#include "../cpu/types.h"

free_memory_block * free_list;
multiboot_memory_map_t * memory_map;
int mmap_size = 0;

void * memcpy(void *dest, const void *src, int len) {

	char *d = dest;
	const char *s = src;
	while (len--) {
		*d++ = *s++;
	}
	return dest;
}

unsigned int free_memory_block_checksum(free_memory_block* free_block) {
	return (uint32_t) free_block->size ^ (uint32_t) free_block->next ^ (uint32_t) free_block->previous;
}

unsigned int allocated_memory_block_checksum(allocated_memory_block* allocated) {
	return -allocated->size;
}

void* find_first_free_available(unsigned int size) {

	if (free_list == 0) {
		return 0;
	}

	if (size > 0) {
		free_memory_block * it_free = free_list;
		free_memory_block * free_use = 0;
		do {
			if (it_free->size >= size) {
				free_use = it_free;
				break;
			}
			it_free = it_free->next;
		} while (it_free != free_list);

		return free_use;
	}
	return 0;
}

void add_free_block(free_memory_block * free_block) {

	if (free_list == 0) {
		free_list = free_block;
	}

	free_block->next = free_list;
	free_block->previous = free_list->previous;
	free_block->previous->next = free_block;
	free_list->previous = free_block;
	free_list = free_block;
	free_list->checksum = free_memory_block_checksum(free_list);
	free_list->next->checksum = free_memory_block_checksum(free_list->next);
	free_list->previous->checksum = free_memory_block_checksum(free_list->previous);
}

void remove_free_block(free_memory_block * free_block) {

	if (free_block == free_block->next) {
		free_list = 0;
		return;
	}

	free_block->next->previous = free_block->previous;
	free_block->next->checksum = free_memory_block_checksum(free_block->next);
	free_block->previous->next = free_block->next;
	free_block->previous->checksum = free_memory_block_checksum(free_block->previous);

	if (free_block == free_list) {
		free_list = free_block->next;
	}

	free_list->checksum = free_memory_block_checksum(free_list);
}

/* Memory not allocated to prevent overrding kernel program memory */
#define KERNEL_MEMORY_SIZE 1024*1024*10

void init_memory_allocator(struct multiboot_tag *tag) {

	free_list = 0;
	multiboot_memory_map_t *mmap;
	for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
		(multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
		mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size))
	{
		mmap_size++;
		if (!mmap->len || !mmap->addr || mmap->type != MULTIBOOT_MEMORY_AVAILABLE) {
			continue;
		}
		free_memory_block * newBlock = (free_memory_block*) (multiboot_uint32_t) mmap->addr + KERNEL_MEMORY_SIZE;
		newBlock->size = mmap->len - KERNEL_MEMORY_SIZE;
		add_free_block(newBlock);
	}

	memory_map = malloc(sizeof(multiboot_memory_map_t) * mmap_size);
	if (!memory_map) {
		printf("Issue with memory. Halting...");
		__asm__ __volatile__("hlt");
	}
	int i = 0;
	for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
		(multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
		mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size))
	{
		memcpy(&memory_map[i], mmap, sizeof(multiboot_memory_map_t));
		i++;
	}
}

void* malloc(unsigned int size) {

	if (size == 0) return 0;

	/* On trouve le premier block libre avec la taille souhaitée */
	free_memory_block * free_use = find_first_free_available(size + sizeof(allocated_memory_block));

	if (free_memory_block_checksum(free_use) != free_use->checksum) {
		printf("Error: Invalid free block checksum %d, %d. Cannot malloc !\n", free_memory_block_checksum(free_use), free_use->checksum);
		return 0;
	}

	if (free_use != 0) {

		unsigned int free_use_size = free_use->size;

		/* Le bloc n'est plus libre donc on l'enlève */
		remove_free_block(free_use);

		/* On alloue le nouveau bloc */
		allocated_memory_block * allocated = (allocated_memory_block *) free_use;
		allocated->size = size;
		allocated->checksum = allocated_memory_block_checksum(allocated);

		/*
		On teste si dans le nouvel espace restant, il est possible de mettre le header au complet pour ne pas empieter sur d'autres zones mémoires
		On perd quelques octets si jamais la condition n'est pas remplie
		*/
		if (free_use_size - size - sizeof(allocated_memory_block) > sizeof(free_memory_block)) {
			/* On crée un nouveau bloc libre avec l'espace restant */
			free_memory_block * newBlock = (free_memory_block*) ((char*) free_use + size + sizeof(allocated_memory_block));
			newBlock->size = free_use_size - size - sizeof(allocated_memory_block);
			add_free_block(newBlock);
		}
		
		/* On retourne le bloc alloue sans le header */
		return (char*) allocated + sizeof(allocated_memory_block);
	}
	else {
		printf("Error: No free block found\n");
		return 0;
	}
}

void free(void* ptr) {

	if (ptr == 0) return;

	/* On récupère le header du bloc alloué */
	allocated_memory_block * allocated = (allocated_memory_block *) ((char*) ptr - sizeof(allocated_memory_block));
	if (allocated->checksum != allocated_memory_block_checksum(allocated)) {
		printf("Error: Invalid allocated block checksum %d, %d. Cannot free !\n", allocated->checksum, allocated_memory_block_checksum(allocated));
		return;
	}
	unsigned int size = allocated->size;

	/* On ajoute un bloc libre si on a la place */
	if (size + sizeof(allocated_memory_block) > sizeof(free_memory_block)) {
		free_memory_block * newBlock = (free_memory_block *) allocated;
		newBlock->size = size + sizeof(allocated_memory_block);
		add_free_block(newBlock);
	}
}

void * memset (void * dest, int val, unsigned len) {
	unsigned char * ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

void * calloc(unsigned int size) {
	void * ptr = malloc(size);
	if (ptr != 0) {
		return memset(ptr, 0, size);
	}
	return 0;
}

void * realloc(void * ptr, unsigned int size) {

	void * newPtr = malloc(size);

	/* On récupère le header du bloc alloué */
	allocated_memory_block * allocated = (allocated_memory_block *) ((char*) ptr - sizeof(allocated_memory_block));
	unsigned int allSize = allocated->size;

	memcpy(newPtr, ptr, allSize);
	free(ptr);

	return newPtr;
}

multiboot_memory_map_t* get_memory_map() {
	return memory_map;
}

int get_memory_map_size() {
	return mmap_size;
}