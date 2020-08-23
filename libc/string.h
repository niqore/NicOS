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

#ifndef STRING_H
#define STRING_H

void strrev(unsigned char *str);

char* itoa(int value, char* buffer, int base);

int strlen(const char *str);

char lower_case(char c);

char upper_case(char c);

int strcmp(const char *p1, const char *p2);

char* format_number_decimals(char *s, int decimals);

#endif