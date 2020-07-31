#ifndef FILESYSTEM_H
#define FILESYSTEM_H

struct path;

typedef struct path {
	char* name;
	struct path* next;
	struct path* previous;
} FILE_PATH;

FILE_PATH* get_file_path(char* current_dir, char* path);

void free_path(FILE_PATH* path);

#endif