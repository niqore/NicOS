#include "elf.h"
#include "../libc/stdio.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"
#include "../libc/shared/libc.h"

LIBC* libc = 0;

int is_elf(unsigned char* magic) {
	return magic[0] == 0x7f && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F';
}

void init_libc() {
	libc = (LIBC*) malloc(sizeof(LIBC));
	libc->printf = &printf;
}

void execute_elf(char* work_dir, unsigned char* file_content, int argc, char** argv) {

	ELF_HEADER* header = (ELF_HEADER*) file_content;

	if (!is_elf(header->magic_number)) {
		printf("Ceci n'est pas un fichier ELF\n");
		return;
	}

	if (header->bits != 1)  {
		printf("Seul le mode 32 bit est supporté pour les ELF\n");
		return;
	}

	if (header->endianess != 1)  {
		printf("Seul le little endian est supporté pour les ELF\n");
		return;
	}

	if (header->elf_type != 2 && header->elf_type != 3)  {
		printf("Ce fichier ELF n'est pas executable\n");
		return;
	}

	if (header->pht_entry_size != sizeof(ELF_PHT_ENTRY)) {
		printf("Taille des entrées PHT incorrecte\n");
		return;
	}

	if (header->sht_entry_size != sizeof(ELF_SHT_ENTRY)) {
		printf("Taille des entrées SHT incorrecte\n");
		return;
	}

	unsigned char* main_addr = file_content + header->pgrm_entry_pos;
	
	//ELF_PHT_ENTRY* pht_entries = (ELF_PHT_ENTRY*) (file_content + header->pht_pos);
	//ELF_SHT_ENTRY* sht_entries = (ELF_SHT_ENTRY*) (file_content + header->sht_pos);
	/*uint32_t shstrtab_offset = sht_entries[header->sht_name_index].offset;
	for (int i = 0; i < header->sht_entry_count; ++i) {
		if (strcmp((char*) (file_content + shstrtab_offset + sht_entries[i].name), ".text") == 0) {
			main_addr = file_content + header->pgrm_entry_pos - sht_entries[i].vaddr + sht_entries[i].offset;
		}
		if (sht_entries[i].type != SHT_NULL) {
			sht_entries[i].vaddr = (uint32_t) file_content + sht_entries[i].offset; // TODO Align
		}
	}
	for (int i = 0; i < header->pht_entry_count; ++i) {
		pht_entries[i].vaddr = (uint32_t) file_content + pht_entries[i].offset; // TODO Align
		pht_entries[i].paddr = pht_entries[i].vaddr;
	}*/

	if (!main_addr) {
		printf("La fonction main n'a pas été trouvée\n");
		return;
	}

	if (libc == 0) {
		init_libc();
	}

	typedef int main_func(int argc, char** argv, char* work_dir, LIBC* libc);
	main_func* main = (main_func*) main_addr;
	int ret = main(argc, argv, work_dir, libc);

	if (ret) {
		printf("Une erreur est survenue. Code: %d\n", ret);
	}

	return;
}