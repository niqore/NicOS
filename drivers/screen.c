#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/string.h"
#include "../libc/stdlib.h"

int get_screen_offset(int col, int row);
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int handle_scrolling(int offset);

int print_char_at(char c, int col, int row, char attribute) {
    
    unsigned char * vidmem = (unsigned char*) VIDEO_ADDRESS;

    if (!attribute) { // Si attribute = 0, on met blanc sur noir par défaut
        attribute = WHITE_ON_BLACK;
    }

    int offset;
    if (row >= 0 && col >= 0) { // On calcule l'offset ou on trouve le pointeur
        offset = get_offset(col, row);
    }
    else {
        offset = get_cursor_offset();
    }

    if (c == '\n') { // Pour une nouvelle ligne, on se place à la fin de la ligne pour passer à la suivante ensuite
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    }
    else { // Sinon on écrit le caractère
        vidmem[offset] = c;
        vidmem[offset + 1] = attribute;
        offset += 2;
    }

    offset = handle_scrolling(offset); // On scroll si on est en bas de l'écran
    set_cursor_offset(offset); // On met à jour le curseur
    return offset;
}

void print_char(char c) {
    print_char_at(c, -1, -1, 0);
}

int get_cursor_offset() {
	// On utlise les ports VGA pour récupérer la position actuelle du curseur
    port_byte_out(REG_SCREEN_CTRL, 14); // On récupère les bits de poids fort de l'offset du curseur (data 14)
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; // On décale pour ensuite ajouter les bits de poids faibles
    port_byte_out(REG_SCREEN_CTRL, 15); // On récupère les bits de poids faible
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; // 1 charactère vaut 2 bytes dans la mémoire
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    unsigned char * screen = (unsigned char *) VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) {
	return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
	return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
	return (offset - (get_offset_row(offset) * 2 * MAX_COLS))/2;
}

int handle_scrolling(int offset) {

    while (offset >= MAX_ROWS * MAX_COLS * 2) {
        for (int i = 1; i < MAX_ROWS; ++i) {
            memcpy((char *) (get_offset(0, i - 1) + VIDEO_ADDRESS),
                    (char *) (get_offset(0, i) + VIDEO_ADDRESS),
                    MAX_COLS * 2);
        }

        char * last_line = (char *) (get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
        for (int i = 0; i < MAX_COLS * 2; i++) {
            last_line[i] = 0;
        }

        offset -= 2 * MAX_COLS;
    }

    return offset;
}

void remove_last_character() {
	int offset = get_cursor_offset();
	if (offset == 0) {
		return;
	}
	unsigned char * vidmem = (unsigned char*) VIDEO_ADDRESS;
	offset -= 2;
	vidmem[offset] = ' ';
	set_cursor_offset(offset);
}