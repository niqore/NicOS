#ifndef MSR_H
#define MSR_H

#include "types.h"

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi);

void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi);

#endif

