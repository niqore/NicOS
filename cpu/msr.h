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

#ifndef MSR_H
#define MSR_H

#include "types.h"

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi);

void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi);

#endif

