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

#ifndef TYPES_H
#define TYPES_H

// Ces types servent à allouer l'espace mémoire nécessaire pour les variables
typedef unsigned long long  uint64_t;
typedef    long long        int64_t;
typedef unsigned int        uint32_t;
typedef          int        int32_t;
typedef unsigned short      uint16_t;
typedef          short      int16_t;
typedef unsigned char       uint8_t;
typedef          char       int8_t;

/* Types for `void *' pointers.  */
typedef long int		    intptr_t;
typedef unsigned long int	uintptr_t;

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#endif