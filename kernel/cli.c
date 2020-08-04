#include "cli.h"
#include "../drivers/screen.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../libc/stdlib.h"
#include "../drivers/pci.h"
#include "../filesystem/filesystem.h"
#include "../filesystem/fat32.h"

char buffer[512];
int buffer_pos = 0;
FILE_PATH* current_directory = 0;

/* Touches spéciales */
int multi_key_down = 0;
int left_shift_down = 0;
int right_shift_down = 0;
int left_ctrl_down = 0;
int left_alt_down = 0;
int altgr_down = 0;

int is_shifting() {
	return left_shift_down || right_shift_down;
}

int is_alting();

int is_ctrling() {
	return left_ctrl_down && !is_alting();
}

int is_alting() {
	return left_alt_down && !is_ctrling();
}

int is_altgring() {
	return (is_ctrling() && is_alting()) || altgr_down;
}

void print_line_beginning() {
	printf("/");
	FILE_PATH* tmp = current_directory;
	while (tmp != 0) {
		printf("%s", tmp->name);
		tmp = tmp->next;
		if (tmp != 0) {
			printf("/");
		}
	}
	printf("$ ");
}

void init_cli() {
	print_line_beginning();
	buffer_pos = 0;
}

int get_buffer_command_length(char* buff) {
	int i = 0;
	while (buff[i] != ' ' && buff[i] != 0) {
		i++;
	}
	return i;
}

int get_buffer_argc(char* buff) {
	int i = 0;
	for (int j = 0; j < 512; ++j) {
		int is_arg = 0;
		while (buff[j] != ' ' && buff[j] != 0) {
			is_arg = 1;
			++j;
		}
		if (is_arg) {
			i++;
		}
	}
	if (i == 0)
		return 0;
	return i - 1;
}

char** get_buffer_argv(char* buff, int command_length, int argc) {
	char** argv = (char**) malloc(argc*sizeof(char*));
	int j = command_length;
	for (int i = 0; i < argc; ++i) {
		while (buff[j] == ' ' || buff[j] == 0) {
			j++;
		}
		int length = 0;
		while (buff[j] != ' ' && buff[j] != 0) {
			length++;
			j++;
		}
		argv[i] = (char*) malloc((length + 1)*sizeof(char));
		memcpy(argv[i], buff + j - length, length);
		argv[i][length] = 0;
	}
	return argv;
}

void execute_buffer() {
	print_char('\n');

	if (buffer_pos < 511) {
		for (int i = buffer_pos; i < 512; ++i) {
			buffer[i] = '\0';
		}
		int cmd_length = get_buffer_command_length(buffer);
		buffer[cmd_length] = 0;
		int argc = get_buffer_argc(buffer);
		char** argv = get_buffer_argv(buffer, cmd_length, argc);
		if (!strcmp(buffer, "raminfo")) {
			print_ram_info();
		}
		else if (!strcmp(buffer, "lspci")) {
			print_pci_devices_info();
		}
		else if (!strcmp(buffer, "cd")) {
			if (argc == 1) {
				FILE_PATH* new_path = argv[0][0] == '/' ? filename_to_path(argv[0], 0) : filename_to_path(argv[0], current_directory);
				FILE_ENTRY* file_entry = get_file_entry(new_path);
				if (file_entry == 0) {
					printf("Aucun dossier avec ce nom\n");
					free_path(new_path);
				}
				else if (!(file_entry->attribute & FAT32_ATTR_SUBDIR)) {
					printf("Ceci n'est pas un dossier\n");
					free_path(new_path);
				}
				else {
					free_path(current_directory);
					current_directory = new_path;
				}
				free(file_entry);
			}
			else {
				printf("Usage correct: cd [path]\n");
			}
		}
		for (int i = 0; i < argc; ++i) {
			free(argv[i]);
		}
	}

	init_cli();
}

void add_buffer_char(char c) {
	if (buffer_pos == 511) {
		return;
	}
	if (is_shifting()) {
		c = upper_case(c);
	}
	buffer[buffer_pos++] = c;
	print_char(c);
}

void handle_backspace() {
	if (buffer_pos == 0) {
		return;
	}
	buffer_pos--;
	remove_last_character();
}

void handle_key(uint8_t scancode) {
	switch (scancode) {
		case 0x1: //Escape
			break;
		case 0x3B: //F1
			break;
		case 0x3C: //F2
			break;
		case 0x3D: //F3
			break;
		case 0x3E: //F4
			break;
		case 0x3F: //F5
			break;
		case 0x40: //F6
			break;
		case 0x41: //F7
			break;
		case 0x42: //F8
			break;
		case 0x43: //F9
			break;
		case 0x44: //F10
			break;
		case 0x57: //F11
			break;
		case 0x58: //F12
			break;
		case 0x46: //Arret defil
			break;
		case 0x29: //oe
			break;
		case 0x2:
			add_buffer_char(is_shifting() ? '1' : '&');
			break;
		case 0x3:
			if (is_shifting()) {
				add_buffer_char('2');
			}
			else if (is_altgring()) {
				add_buffer_char('~');
			}
			break;
		case 0x4:
			if (is_shifting()) {
				add_buffer_char('3');
			}
			else if (is_altgring()) {
				add_buffer_char('#');
			}
			else {
				add_buffer_char('"');
			}
			break;
		case 0x5:
			if (is_shifting()) {
				add_buffer_char('4');
			}
			else if (is_altgring()) {
				add_buffer_char('{');
			}
			else {
				add_buffer_char('\'');
			}
			break;
		case 0x6:
			if (is_shifting()) {
				add_buffer_char('5');
			}
			else if (is_altgring()) {
				add_buffer_char('[');
			}
			else {
				add_buffer_char('(');
			}
			break;
		case 0x7:
			if (is_shifting()) {
				add_buffer_char('6');
			}
			else if (is_altgring()) {
				add_buffer_char('|');
			}
			else {
				add_buffer_char('-');
			}
			break;
		case 0x8:
			if (is_shifting()) {
				add_buffer_char('7');
			}
			else if (is_altgring()) {
				add_buffer_char('`');
			}
			break;
		case 0x9:
			if (is_shifting()) {
				add_buffer_char('8');
			}
			else if (is_altgring()) {
				add_buffer_char('\\');
			}
			else {
				add_buffer_char('_');
			}
			break;
		case 0xA:
			if (is_shifting()) {
				add_buffer_char('9');
			}
			else if (is_altgring()) {
				add_buffer_char('^');
			}
			break;
		case 0xB:
			if (is_shifting()) {
				add_buffer_char('0');
			}
			else if (is_altgring()) {
				add_buffer_char('@');
			}
			break;
		case 0xC:
			if (is_altgring()) {
				add_buffer_char(']');
			}
			else {
				add_buffer_char(')');
			}
			break;
		case 0xD:
			if (is_shifting()) {
				add_buffer_char('+');
			}
			else if (is_altgring()) {
				add_buffer_char('}');
			}
			else {
				add_buffer_char('=');
			}
			break;
		case 0xE:
			handle_backspace();
			break;
		case 0xF: //Tab
			break;
		case 0x10:
			add_buffer_char('a');
			break;
		case 0x11:
			add_buffer_char('z');
			break;
		case 0x12:
			add_buffer_char('e');
			break;
		case 0x13:
			add_buffer_char('r');
			break;
		case 0x14:
			add_buffer_char('t');
			break;
		case 0x15:
			add_buffer_char('y');
			break;
		case 0x16:
			add_buffer_char('u');
			break;
		case 0x17:
			add_buffer_char('i');
			break;
		case 0x18:
			add_buffer_char('o');
			break;
		case 0x19:
			add_buffer_char('p');
			break;
		case 0x1A:
			add_buffer_char('^');
			break;
		case 0x1B:
			add_buffer_char('$');
			break;
		case 0x1C: //Enter
			execute_buffer();
			break;
		case 0x1E:
			add_buffer_char('q');
			break;
		case 0x1F:
			add_buffer_char('s');
			break;
		case 0x20:
			add_buffer_char('d');
			break;
		case 0x21:
			add_buffer_char('f');
			break;
		case 0x22:
			add_buffer_char('g');
			break;
		case 0x23:
			add_buffer_char('h');
			break;
		case 0x24:
			add_buffer_char('j');
			break;
		case 0x25:
			add_buffer_char('k');
			break;
		case 0x26:
			add_buffer_char('l');
			break;
		case 0x27:
			add_buffer_char('m');
			break;
		case 0x28:
			add_buffer_char('%');
			break;
		case 0x2B:
			add_buffer_char('*');
			break;
		case 0x2C:
			add_buffer_char('w');
			break;
		case 0x2D:
			add_buffer_char('x');
			break;
		case 0x2E:
			add_buffer_char('c');
			break;
		case 0x2F:
			add_buffer_char('v');
			break;
		case 0x30:
			add_buffer_char('b');
			break;
		case 0x31:
			add_buffer_char('n');
			break;
		case 0x32:
			if (is_shifting()) {
				add_buffer_char('?');
			}
			else {
				add_buffer_char(',');
			}
			break;
		case 0x33:
			if (is_shifting()) {
				add_buffer_char('.');
			}
			else {
				add_buffer_char(';');
			}
			break;
		case 0x34:
			if (is_shifting()) {
				add_buffer_char('/');
			}
			else {
				add_buffer_char(':');
			}
			break;
		case 0x35:
			add_buffer_char('!');
			break;
		case 0x37:
			add_buffer_char('*');
			break;
		case 0x39:
			add_buffer_char(' ');
			break;
		case 0x3A: //Caps lock
			break;
		case 0x45: //Verr num
			break;
		case 0x2A: //Left shift down
			left_shift_down = 1;
			break;
		case 0xAA: //Left shit up
			left_shift_down = 0;
			break;
		case 0x36: //Right shift down
			right_shift_down = 1;
			break;
		case 0xB6: //Right shit up
			right_shift_down = 0;
			break;
		case 0x1D: //Left ctrl down
			left_ctrl_down = 1;
			break;
		case 0x9D: //Left ctrl up
			left_ctrl_down = 0;
			break;
		case 0x38:
			if (multi_key_down) {
				altgr_down = 1;
			}
			else {
				left_alt_down = 1;
			}
			break;
		case 0x47:
			add_buffer_char('7');
			break;
		case 0x48:
			add_buffer_char('8');
			break;
		case 0x49:
			add_buffer_char('9');
			break;
		case 0x4A:
			add_buffer_char('-');
			break;
		case 0x4B:
			add_buffer_char('4');
			break;
		case 0x4C:
			add_buffer_char('5');
			break;
		case 0x4D:
			add_buffer_char('6');
			break;
		case 0x4E:
			add_buffer_char('+');
			break;
		case 0x4F:
			add_buffer_char('1');
			break;
		case 0x50:
			add_buffer_char('2');
			break;
		case 0x51:
			add_buffer_char('3');
			break;
		case 0x52:
			add_buffer_char('0');
			break;
		case 0x53:
			add_buffer_char('.');
			break;
		case 0xB8:
			if (multi_key_down) {
				altgr_down = 0;
			}
			else {
				left_alt_down = 0;
			}
			break;
		case 0x56:
			if (is_shifting()) {
				add_buffer_char('>');
			}
			else if (is_altgring()) {
				add_buffer_char('|');
			}
			else {
				add_buffer_char('<');
			}
			break;
		case 0xE0:
			multi_key_down = 1;
			break;
		default:
			break;

	}
	multi_key_down = 0;
}