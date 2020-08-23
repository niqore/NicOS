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