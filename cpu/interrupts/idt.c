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

#include "idt.h"

/* Attribue les valeur du gate */
void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = low_16(handler); // Les 16 bits de poids faible de l'adresse d'appel du handler
    idt[n].sel = KERNEL_CS; // Selecteur du kernel
    idt[n].always0 = 0; // Inutilisé, doit être à 0
    idt[n].flags = 0x8E; // Flags (voir README.md)
    idt[n].high_offset = high_16(handler); // 16 bits de poids fort
}

/* Enregistrement de l'idt dans le registre */
void set_idt() {
    idt_reg.base = (uint32_t) &idt; // Adresse de l'IDT
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1; // Taille de l'IDT
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}