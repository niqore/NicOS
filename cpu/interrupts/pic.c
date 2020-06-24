#include "pic.h"
#include "../types.h"
#include "../cpuid.h"
#include "../msr.h"

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_ENABLE 0x800

int apic_available() {
   uint32_t eax, ebx, ecx, edx;
   native_cpuid(&eax, &ebx, &ecx, &edx);
   return edx & CPUID_FEAT_EDX_APIC;
}

void cpu_set_apic_base(uintptr_t apic) {
   uint32_t edx = 0;
   uint32_t eax = (apic & 0xfffff000) | IA32_APIC_BASE_MSR_ENABLE;
   cpuSetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

uintptr_t cpu_get_apic_base() {
   uint32_t eax, edx;
   cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
   return (eax & 0xfffff000);
}

void enable_apic() {
    cpu_set_apic_base(cpu_get_apic_base());
    uint32_t * ptr = (uint32_t *) 0xfee000f0;
    *ptr |= 0x100; // On met le bit 8 de l'APIC à 1
}