/*
Permet d'éviter au kernel à sauter en kernel.c:0x00
En effet, la place de ce code dépend du linker, il peut se trouver aussi bien au début qu'à la fin.
En mettant cette fonction, comme l'ordre est gardé, on s'assure de passer dans main()
*/
void dummy_test_entrypoint() {
}

#include "../drivers/screen.h"
#include "utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/pic.h"

void main() {
	clear_screen();
	print_string("Bienvenue sur NicOS !\nLe CPU est actuellement en mode 32-bit\n\n");

	isr_install();

	if (apic_available()) {
		enable_apic();
	}
}
