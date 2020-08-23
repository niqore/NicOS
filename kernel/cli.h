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

#ifndef CLI_H
#define CLI_H

#include "../cpu/types.h"

void init_cli();

void handle_key(uint8_t scancode);

uint8_t get_last_key();

void reset_last_key();

void set_in_elf(int in);

#endif