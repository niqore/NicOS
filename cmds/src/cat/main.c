#include "libc.h" 

int main(int argc, char** argv, char* work_dir, LIBC* libc) {

	FILE_PATH* path; 
	if (argc == 1) {
		FILE_PATH* tmp = libc->filename_to_path(work_dir, 0);
		path = libc->filename_to_path(argv[0], tmp);
		libc->free_path(tmp);
	}
	else if (argc == 0) {
		libc->printf("Utilisation: cat [fichier]\n");
		return 1;
	}

	FILE_ENTRY* file_entry = libc->get_file_entry(path);
	libc->free_path(path);
	if (file_entry == 0) {
		libc->printf("Le chemin n'existe pas\n");
		return 2;
	}
	if (!(file_entry->attribute & FAT32_ATTR_ARCHIVE)) {
		libc->printf("N'est pas un fichier\n");
		return 3;
	}

	unsigned char* content = libc->read_fat32_file(file_entry);

	uint32_t i = 0;
	unsigned char* content_ptr = content;
	while (i != file_entry->file_size) {
		libc->printf("%c", *(content_ptr++));
		i++;
	}

	libc->free(file_entry);
	libc->free(content);

	libc->printf("\n");

	return 0;
}