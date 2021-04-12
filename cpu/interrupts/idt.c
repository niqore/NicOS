#include "idt.h"

/* Attribue les valeur du gate */
void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = low_16(handler); // Les 16 bits de poids faible de l'adresse d'appel du handler
    idt[n].sel = KERNEL_CS; // Selecteur du kernel
    idt[n].always0 = 0; // Inutilisé, doit être à 0
    idt[n].flags = 0x8E; // Flags
    idt[n].high_offset = high_16(handler); // 16 bits de poids fort
}

/* Enregistrement de l'idt dans le registre */
void set_idt() {
    idt_reg.base = (uint32_t) &idt; // Adresse de l'IDT
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1; // Taille de l'IDT
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}