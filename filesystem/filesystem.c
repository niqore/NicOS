#include "filesystem.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"

FILE_PATH* copy_file_path(FILE_PATH* path) {

	if (path == 0)
		return 0;

	FILE_PATH* first = 0;
	FILE_PATH* cur_path = 0;

	while (path != 0) {
		FILE_PATH* new_path_tmp = (FILE_PATH*) malloc(sizeof(FILE_PATH));
		memcpy(new_path_tmp, path, sizeof(FILE_PATH));
		int name_length = strlen(path->name);
		new_path_tmp->name = (char*) malloc((name_length + 1)*sizeof(char));
		memcpy(new_path_tmp->name, path->name, name_length + 1);
		if (first == 0) {
			first = new_path_tmp;
		}
		new_path_tmp->previous = cur_path;
		if (cur_path != 0) {
			cur_path->next = new_path_tmp;
		}
		cur_path = new_path_tmp;
		path = path->next;
	}
	if (cur_path->next != 0) {
		cur_path->next = 0;
	}

	return first;
}

FILE_PATH* filename_to_path(char* dir, FILE_PATH* base_path) {
	char* cd = dir;
	FILE_PATH* first = copy_file_path(base_path);
	FILE_PATH* cur_path = first;
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

char* path_to_string(FILE_PATH* path) {

	if (path == 0) {
		char* root = (char*) malloc(2);
		root[0] = '/';
		root[1] = 0;
		return root;
	}

	FILE_PATH* tmp = path;
	int length = 0;
	int count = 0;
	while (tmp != 0) {
		length += strlen(tmp->name);
		count++;
		tmp = tmp->next;
	}

	char* str = (char*) malloc(sizeof(char)*(length + count + 2));

	str[0] = '/';
	int pos = 1;
	tmp = path;
	while (tmp != 0) {
		int strl = strlen(tmp->name);
		memcpy(str + pos, tmp->name, strl);
		pos += strl;
		str[pos] = '/';
		pos++;
		tmp = tmp->next;
	}
	str[pos] = 0;
	return str;
}

void free_path(FILE_PATH* path) {
	if (path == 0) return;
	FILE_PATH* next = path;
	while (next != 0) {
		FILE_PATH* n = next->next;
		free(next->name);
		free(next);
		next = n;
	}
}