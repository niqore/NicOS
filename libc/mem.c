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

void init_memory_allocator() {
	free_list = (void*) FIRST_FREE_ADDR;
	free_list->size = MAX_FREE_ADDR - FIRST_FREE_ADDR;
	free_list->next = free_list;
	free_list->previous = free_list;
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
	free_list->previous = free_block;
	free_list = free_block;
}

void remove_free_block(free_memory_block * free_block) {

	if (free_block == free_block->next) {
		free_list = 0;
		return;
	}

	free_block->next->previous = free_block->previous;
	free_block->previous->next = free_block->next;
}

void* malloc(unsigned int size) {

	/* On trouve le premier block libre avec la taille souhaitée */
	free_memory_block * free_use = find_first_free_available(size + sizeof(allocated_memory_block));

	if (free_use != 0) {

		unsigned int free_use_size = free_use->size;

		/* Le bloc n'est plus libre donc on l'enlève */
		remove_free_block(free_use);

		/* On alloue le nouveau bloc */
		allocated_memory_block * allocated = (allocated_memory_block *) free_use;
		allocated->size = size;

		/*
		On teste si dans le nouvel espace restant, il est possible de mettre le header au complet pour ne pas empieter sur d'autres zones mémoires
		On perd quelques octets si jamais la condition n'est pas remplie
		*/
		if (free_use_size - size - sizeof(free_memory_block) - 1 > 0) {
			/* On crée un nouveau bloc libre avec l'espace restant */
			free_memory_block * newBlock = free_use + size + sizeof(allocated_memory_block);
			newBlock->size = free_use_size - size - sizeof(allocated_memory_block);
			add_free_block(newBlock);
		}
		
		/* On retourne le bloc alloue sans le header */
		return allocated + sizeof(allocated_memory_block);
	}
	else {
		return 0;
	}
}