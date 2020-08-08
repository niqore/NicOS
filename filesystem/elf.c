#include "elf.h"

int is_elf(unsigned char* file_content) {
	return file_content[0] == 0x7f && file_content[1] == 'E' && file_content[2] == 'L' && file_content[3] == 'F';
}

int execute_elf(unsigned char* file_content) {

	if (!is_elf(file_content))
		return 1;

	return 0;
}