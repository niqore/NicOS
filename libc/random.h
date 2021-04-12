#ifndef RANDOM_H
#define RANDOM_H

#include "../cpu/types.h"

uint32_t rand(void);
 
void srand(uint32_t seed);

uint32_t random_seed();

uint32_t randint(uint32_t min, uint32_t max);

#endif