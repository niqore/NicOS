#include "elf.h"
#include "../libc/stdio.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"
#include "../libc/shared/libc.h"
#include "../kernel/cli.h"
#include "../drivers/screen.h"
#include "../libc/random.h"
#include "../cpu/timer.h"

LIBC* libc = 0;
QUEUED_ELF* elf_queue = 0;

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

	libc->strrev = &strrev;
	libc->itoa = &itoa;
	libc->strlen = &strlen;
	libc->lower_case = &lower_case;
	libc->upper_case = &upper_case;
	libc->strcmp = &strcmp;
	libc->format_number_decimals = &format_number_decimals;

	libc->get_last_key = &get_last_key;
	libc->reset_last_key = &reset_last_key;

	libc->print_char_at = &print_char_at;
	libc->clear_screen = &clear_screen;

	libc->rand = &rand;
	libc->srand = &srand;
	libc->random_seed = &random_seed;
	libc->randint = &randint;

	libc->sleep = &sleep;

	libc->get_device_list = &get_device_list;
	libc->get_device_count = &get_device_count;

	libc->get_memory_map = &get_memory_map;
	libc->get_memory_map_size = &get_memory_map_size;
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

int execute_elf_queue() {
	if (elf_queue != 0) {
		srand(random_seed());
		set_in_elf(1);
		execute_elf(elf_queue->wd, elf_queue->file_content, elf_queue->argc, elf_queue->argv);
		set_in_elf(0);
		free(elf_queue->wd);
		free(elf_queue->file_content);
		for (int i = 0; i < elf_queue->argc; ++i) {
			free(elf_queue->argv[i]);
		}
		if (elf_queue->argv != 0) {
			free(elf_queue->argv);
		}
		free(elf_queue);
		elf_queue = 0;
		return 1;
	}
	return 0;
}

void add_elf_to_queue(QUEUED_ELF* queued) {
	elf_queue = queued;
}