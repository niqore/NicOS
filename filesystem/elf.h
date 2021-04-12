#ifndef ELF_H
#define ELF_H

#include "../cpu/types.h"

typedef struct _elf_header {

	unsigned char magic_number[4];
	uint8_t bits; // 1 = 32 bit, 2 = 64 bit
	uint8_t endianess; // 1 = little endian, 2 = big endian
	uint8_t header_version;
	uint8_t os_abi; // Normalement 0
	unsigned char padding[8];
	uint16_t elf_type; // 1 = relocatable, 2 = executable, 3 = shared, 4 = core
	uint16_t instruction_set;
	uint32_t elf_version;
	uint32_t pgrm_entry_pos;
	uint32_t pht_pos; // Program Header Table Position
	uint32_t sht_pos; // Section Header Table Position
	uint32_t flags;
	uint16_t header_size;
	uint16_t pht_entry_size; // Program Header Table Entry Size
	uint16_t pht_entry_count;
	uint16_t sht_entry_size; // Section Header Table Entry Size
	uint16_t sht_entry_count;
	uint16_t sht_name_index; // Index in section header table with the section names 

}__attribute__((packed)) ELF_HEADER;

typedef struct _elf_pht_entry {

	uint32_t type; // Voir PT_ en dessous
	uint32_t offset; // Offset du segment
	uint32_t vaddr; // Virtual address
	uint32_t paddr; // Physical address
	uint32_t filesz; // Taille du segment en octets. Peut valoir 0
	uint32_t memsz; // Taille du segment en mémoire. Peut valoir 0
	uint32_t flags;
	uint32_t align; // 0 et 1 = pas d'alignement. Sinon positif, puissance de 2, avec vaddr = offset % align

}__attribute__((packed)) ELF_PHT_ENTRY;

typedef struct _elf_sht_entry {

	uint32_t name; // Offset du nom dans la section .shstrtab
	uint32_t type; // Voir SHT_ en dessous
	uint32_t flags; // Voir SHF_ en dessous
	uint32_t vaddr; // Virtual address
	uint32_t offset; // Offset de la section
	uint32_t size; // Taille en octets, peut valoir 0
	uint32_t link; // Index de la section associée
	uint32_t info;
	uint32_t addralign; // Puissance de 2
	uint32_t entsize; // Taille en octets de chaque entrée, pour les sections avec taille définie. Sinon vaut 0

}__attribute__((packed)) ELF_SHT_ENTRY;

typedef struct _queued_elf {

	char* wd;
	unsigned char* file_content;
	int argc;
	char** argv;

} QUEUED_ELF;

#define PT_NULL 0x00
#define PT_LOAD 0x01
#define PT_DYNAMIC 0x02
#define PT_INTERP 0x03
#define PT_NOTE 0x04
#define PT_SHLIB 0x05
#define PT_PHDR 0x06
#define PT_TLS 0x07

#define SHT_NULL 0x00
#define SHT_PROGBITS 0x01
#define SHT_SYMTAB 0x02
#define SHT_STRTAB 0x03
#define SHT_RELA 0x04
#define SHT_HASH 0x05
#define SHT_DYNAMIC 0x06
#define SHT_NOTE 0x07
#define SHT_NOBITS 0x08
#define SHT_REL 0x09
#define SHT_SHLIB 0x0A
#define SHT_DYNSYM 0x0B
#define SHT_INIT_ARRAY 0x0E
#define SHT_FINI_ARRAY 0x0F
#define SHT_PREINIT_ARRAY 0x10
#define SHT_GROUP 0x11
#define SHT_SYMTAB_SHNDX 0x12
#define SHT_NUM 0x13

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400

void execute_elf(char* wd, unsigned char* file_content, int argc, char** argv);

void add_elf_to_queue(QUEUED_ELF* queued);

int execute_elf_queue();

#endif