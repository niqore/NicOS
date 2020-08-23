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

#ifndef TIMER_H
#define TIMER_H

#include "types.h"

uint32_t get_current_tick();

void init_timer(uint32_t freq);

void sleep(uint32_t ms);

#endif