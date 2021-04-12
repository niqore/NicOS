#include "libc.h" 

int main(int argc, char** argv, char* work_dir, LIBC* libc) {

	FILE_PATH* path; 
	if (argc == 1) {
		FILE_PATH* tmp = libc->filename_to_path(work_dir, 0);
		path = libc->filename_to_path(argv[0], tmp);
		libc->free_path(tmp);
	}
	else if (argc == 0) {
		path = libc->filename_to_path(work_dir, 0);
	}
	else {
		libc->printf("Trop d'arguments\n");
		return 1;
	}

	FILE_ENTRY* file_entry = libc->get_file_entry(path);
	libc->free_path(path);
	if (file_entry == 0) {
		libc->printf("Le chemin n'existe pas\n");
		return 2;
	}
	if (!(file_entry->attribute & FAT32_ATTR_SUBDIR)) {
		libc->printf("N'est pas un dossier\n");
		return 0;
	}

	uint32_t cur_sector = ((uint32_t) file_entry->cluster_high_bytes << 16) + file_entry->cluster_low_bytes;
	while (cur_sector < 0xffffef) {
		libc->free(file_entry);
		file_entry = libc->get_dir_entries(cur_sector);
		for (int i = 0; i < 16; ++i) {
			FILE_ENTRY ent = file_entry[i];
			if (ent.short_name[0] == 0 || (unsigned char) ent.short_name[0] == 0xE5) {
				continue;
			}
			if (ent.attribute == 0xF) { // is vfat
				int seq_num = (*((VFAT_ENTRY*) &ent)).sequence_number & 0x1F;
				i += seq_num;
				for (int j = i - 1; j > i - seq_num - 1; --j) {
					VFAT_ENTRY vfat = *((VFAT_ENTRY*) &file_entry[j]);
					if (vfat.char1 != 0 && (unsigned char) vfat.char1 != 0xFF)
						libc->printf("%c", vfat.char1);
					if (vfat.char2 != 0 && (unsigned char) vfat.char2 != 0xFF)
						libc->printf("%c", vfat.char2);
					if (vfat.char3 != 0 && (unsigned char) vfat.char3 != 0xFF)
						libc->printf("%c", vfat.char3);
					if (vfat.char4 != 0 && (unsigned char) vfat.char4 != 0xFF)
						libc->printf("%c", vfat.char4);
					if (vfat.char5 != 0 && (unsigned char) vfat.char5 != 0xFF)
						libc->printf("%c", vfat.char5);
					if (vfat.char6 != 0 && (unsigned char) vfat.char6 != 0xFF)
						libc->printf("%c", vfat.char6);
					if (vfat.char7 != 0 && (unsigned char) vfat.char7 != 0xFF)
						libc->printf("%c", vfat.char7);
					if (vfat.char8 != 0 && (unsigned char) vfat.char8 != 0xFF)
						libc->printf("%c", vfat.char8);
					if (vfat.char9 != 0 && (unsigned char) vfat.char9 != 0xFF)
						libc->printf("%c", vfat.char9);
					if (vfat.char10 != 0 && (unsigned char) vfat.char10 != 0xFF)
						libc->printf("%c", vfat.char10);
					if (vfat.char11 != 0 && (unsigned char) vfat.char11 != 0xFF)
						libc->printf("%c", vfat.char11);
					if (vfat.char12 != 0 && (unsigned char) vfat.char12 != 0xFF)
						libc->printf("%c", vfat.char12);
					if (vfat.char13 != 0 && (unsigned char) vfat.char13 != 0xFF)
						libc->printf("%c", vfat.char13);
				}
			}
			else {
				for (int j = 0; j < 8; ++j) {
					if (ent.short_name[j] == ' ') {
						break;
					}
					libc->printf("%c", libc->lower_case(ent.short_name[j]));
				}
				if (ent.extension[0] != ' ' || ent.extension[1] != ' ' || ent.extension[2] != ' ') {
					libc->printf(".");
					for (int j = 0; j < 3; ++j) {
						if (ent.extension[j] != ' ') {
							libc->printf("%c", libc->lower_case(ent.extension[j]));
						}
					}
				}
			}
			libc->printf("  ");
		}
		cur_sector = libc->get_next_sector(cur_sector);
	}
	libc->printf("\n");

	libc->free(file_entry);

	return 0;
}