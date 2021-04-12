#include "random.h"
#include "../cpu/timer.h"

static uint32_t next = 1;
 
uint32_t rand(void) { // RAND_MAX assumed to be 32767
    next = next * 1103515245 + 12345;
    return (uint32_t) next;
}
 
void srand(uint32_t seed) {
    next = seed;
}

uint32_t random_seed() {
	return get_current_tick() * 1818177987 + 5417;
}

/* Generates a random uint32_t between min and max, max excluded */
uint32_t randint(uint32_t min, uint32_t max) {
	return (rand() % (max - min)) + min;
}