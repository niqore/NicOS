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

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Couleurs
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

// ports I/O
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

int print_char_at(char c, int col, int row, char attribute);
void clear_screen();
void print_char(char c);
void remove_last_character();