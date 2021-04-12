#ifndef CLI_H
#define CLI_H

#include "../cpu/types.h"

void init_cli();

void handle_key(uint8_t scancode);

uint8_t get_last_key();

void reset_last_key();

void set_in_elf(int in);

#endif