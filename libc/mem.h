/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Nicolas Virard <nicolas.virard@ecole.ensicaen.fr> 
 * @version     0.0.1 - 23-08-2020
 */

#ifndef MEM_H
#define MEM_H

#include "../kernel/multiboot2.h"

typedef struct _allocated_memory_block allocated_memory_block;

struct _allocated_memory_block {
	unsigned int size;
	unsigned int checksum;
}__attribute__((packed));

typedef struct _free_memory_block free_memory_block;

struct _free_memory_block {
	unsigned int size;
	free_memory_block * next;
	free_memory_block * previous;
	unsigned int checksum;
}__attribute__((packed));

multiboot_memory_map_t* get_memory_map();
int get_memory_map_size();

#endif