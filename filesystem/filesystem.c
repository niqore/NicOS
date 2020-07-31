#include "filesystem.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"

FILE_PATH* get_file_path(char* current_dir, char* path) {
	char* cd = current_dir;
	FILE_PATH* first = 0;
	FILE_PATH* cur_path = 0;
	int cd_len = strlen(current_dir);
	int i = 0;
	while (cd[i]) {
		if (cd[i] == '/') {
			i++;
			continue;
		}
		int l = 0;
		while (cd[i] != 0 && cd[i] != '/') {
			l++;
			i++;
		}
		if (l == 2 && cd[i - 2] == '.' && cd[i - 1] == '.') {
			if (cur_path->previous != 0) {
				FILE_PATH* prev_tmp = cur_path->previous;
				cur_path->previous->next = 0;
				free_path(cur_path);
				cur_path = prev_tmp;
			}
			continue;
		}
		if (l == 1 && cd[i - 1] == '.') {
			continue;
		}
		FILE_PATH* new_path = (FILE_PATH*) malloc(sizeof(FILE_PATH));
		new_path->name = (char*) malloc(sizeof(char)*(l+1));
		memcpy(new_path->name, cd + i - l, sizeof(char)*l);
		new_path->name[l] = 0;
		new_path->next = 0;
		new_path->previous = 0;
		if (cur_path == 0) {
			cur_path = new_path;
			first = cur_path;
		}
		else {
			new_path->previous = cur_path;
			cur_path->next = new_path;
			cur_path = new_path;
		}
		if (i == cd_len && cd == current_dir) {
			cd = path;
			i = 0;
		}
	}
	return first;
}

void free_path(FILE_PATH* path) {
	if (path == 0) return;
	FILE_PATH* next = path->next;
	while (path != 0) {
		free(path->name);
		free(path);
		path = next;
		next = path->next;
	}
}