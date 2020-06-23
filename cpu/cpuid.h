#ifndef CPUID_H
#define CPUID_H

#define CPUID_FEAT_EDX_APIC (1 << 9)

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) {
	asm volatile("cpuid"
		: "=a" (*eax),
		"=b" (*ebx),
		"=c" (*ecx),
		"=d" (*edx)
	: "0" (*eax), "2" (*ecx));
}

#endif