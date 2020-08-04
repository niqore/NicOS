#include "filesystem.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"

FILE_PATH* filename_to_path(char* dir, FILE_PATH* base_path) {
	char* cd = dir;
	FILE_PATH* first = base_path;
	FILE_PATH* cur_path = base_path;
	while (cur_path != 0 && cur_path->next != 0) {
		cur_path = cur_path->next;
	}
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
			else {
				if (cur_path != 0) {
					free_path(cur_path);
				}
				cur_path = 0;
				first = 0;
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
	}
	return first;
}

FILE_PATH* combine_paths(FILE_PATH* path1, FILE_PATH* path2) {
	if (path1 == 0)
		return path2;
	FILE_PATH* cur_path = path1;
	while (cur_path->next != 0) {
		cur_path = cur_path->next;
	}
	cur_path->next = path2;
	return path1;
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