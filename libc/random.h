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

#ifndef RANDOM_H
#define RANDOM_H

#include "../cpu/types.h"

uint32_t rand(void);
 
void srand(uint32_t seed);

uint32_t random_seed();

uint32_t randint(uint32_t min, uint32_t max);

#endif