/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Nicolas Virard <nicolas.virard@ecole.ensicaen.fr> 
 * @version     0.0.1 - 23-08-2020
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

struct path;

typedef struct path {
	char* name;
	struct path* next;
	struct path* previous;
} FILE_PATH;

FILE_PATH* filename_to_path(char* dir, FILE_PATH* base_dir);

FILE_PATH* combine_paths(FILE_PATH* path1, FILE_PATH* path2);

void free_path(FILE_PATH* path);

char* path_to_string(FILE_PATH* path);

#endif