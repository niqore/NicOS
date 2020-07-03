#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include "../cpu/types.h"

// Structure d'un segment de RAM
typedef struct SMAP_entry {
 
	uint32_t BaseL; // Adresse de base 64 bits (2x32)
	uint32_t BaseH;
	uint32_t LengthL; // longueur 64 bits
	uint32_t LengthH;
	uint32_t Type; // Type d'entrée
	uint32_t ACPI; // Extended
 
}__attribute__((packed)) SMAP_entry_t;

#define MEM_MAP_ENT_ADDR 0x8000
#define MEM_MAP_STRUCTS_ADDR 0x8004

#endif