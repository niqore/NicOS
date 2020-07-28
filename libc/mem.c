#include "mem.h"
#include "stdlib.h"
#include "../drivers/screen.h"
#include "string.h"
#include "stdio.h"

void * memcpy(void *dest, const void *src, int len) {

	char *d = dest;
	const char *s = src;
	while (len--) {
		*d++ = *s++;
	}
	return dest;
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
}

void remove_free_block(free_memory_block * free_block) {

	if (free_block == free_block->next) {
		free_list = 0;
		return;
	}

	free_block->next->previous = free_block->previous;
	free_block->previous->next = free_block->next;
}

void init_memory_allocator(SMAP_entry_t* map, int entries) {

	free_list = 0;
	for (int i = 0; i < entries; ++i) {
		SMAP_entry_t m = map[i];
		if ((!m.LengthH && !m.LengthL) || m.Type != 1 || m.BaseL == 0) {
			continue;
		}
		free_memory_block * newBlock = (free_memory_block*) m.BaseL;
		newBlock->size = m.LengthL; // Le système est en 32 bits donc on ne devrait pas dépasser 32 bits en taille
		add_free_block(newBlock);
	}
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
		if (free_use_size > sizeof(free_memory_block) + size) {
			/* On crée un nouveau bloc libre avec l'espace restant */
			free_memory_block * newBlock = (free_memory_block*) ((char*) free_use + size + sizeof(allocated_memory_block));
			newBlock->size = free_use_size - size - sizeof(allocated_memory_block);
			add_free_block(newBlock);
		}
		
		/* On retourne le bloc alloue sans le header */
		return (char*) allocated + sizeof(allocated_memory_block);
	}
	else {
		return 0;
	}
}

void free(void* ptr) {

	/* On récupère le header du bloc alloué */
	allocated_memory_block * allocated = (allocated_memory_block *) ((char*) ptr - sizeof(allocated_memory_block));
	unsigned int size = allocated->size;

	/* On ajoute un bloc libre si on a la place */
	if (sizeof(free_memory_block) - size - sizeof(allocated_memory_block) > 0) {
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

void print_ram_info(SMAP_entry_t* map, int entries) {

	char itoa_buffer[16];
	unsigned int amount = 0;

	for (int i = 0; i < entries; ++i) {
		SMAP_entry_t m = map[i];
		if (m.BaseL == 0xfffc0000) {
			continue;
		}
		amount += m.LengthL;
		printf("0x");
		printf(format_number_decimals(itoa(m.BaseH, itoa_buffer, 16), 8));
		printf(format_number_decimals(itoa(m.BaseL, itoa_buffer, 16), 8));
		printf(" -> 0x");
		printf(format_number_decimals(itoa(m.BaseH + m.LengthH, itoa_buffer, 16), 8));
		printf(format_number_decimals(itoa(m.BaseL + m.LengthL, itoa_buffer, 16), 8));
		printf("; Type = ");
		printf(itoa(m.Type, itoa_buffer, 16));
		if (m.Type == 1) {
			printf(" (Usable)");
		}
		else if (m.Type == 2) {
			printf(" (Reserved)");
		}
		else {
			printf(" (Undefined)");
		}
		printf("; ACPI = ");
		printf(itoa(m.ACPI, itoa_buffer, 16));
		print_char('\n');
	}
	printf("Total size: ");
	printf(itoa(amount / 1000000, itoa_buffer, 10));
	printf(" MB (");
	printf(itoa(amount, itoa_buffer, 10));
	printf(" B)\n");
}