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

	libc->filename_to_path = &filename_to_path;
	libc->get_file_entry = &get_file_entry;
	libc->get_dir_entries = &get_dir_entries;
	libc->get_next_sector = &get_next_sector;
	libc->combine_paths = &combine_paths;
	libc->free_path = &free_path;
	libc->read_fat32_file = &read_fat32_file;

	libc->malloc = &malloc;
	libc->free = &free;

	libc->lower_case = &lower_case;
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