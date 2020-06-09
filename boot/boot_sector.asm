[org 0x7c00] ; Indique à l'assembleur que l'offset est celui du bootsector. Il va donc décaler toutes données du code par 0x7c00

mov bp, 0x9000 ; pile
mov sp, bp

mov bx, WELCOME
call print
call print_nl
call print_nl

mov bx, PROT_MODE_LOAD_MSG
call print
call print_nl
call print_nl

call switch_to_pm

; Boucle infinie. Ne sera jamais exécutée
jmp $

%include "boot/boot_sector_print.asm"
%include "boot/32bit-gdt.asm"
%include "boot/32bit-print.asm"
%include "boot/32bit-switch.asm"
;%include "boot/boot_sector_print_hex.asm"
;%include "boot/boot_sector_disk.asm"

[bits 32]
BEGIN_PM: ; Point d'entrée en mode 32 bits
	mov ebx, PROT_MODE_ENTERED_MSG
	call print_string_pm
	jmp $

WELCOME:
	db 'Bienvenue sur NicOS !', 0; Le 0 sert à terminer la chaine de caractère
	
PROT_MODE_LOAD_MSG:
	db 'Passage en mode 32-bit protected...', 0
	
PROT_MODE_ENTERED_MSG:
	db 'Entree en mode 32-bit finie !', 0

; Bootsector
; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre magic afin que ce soit bootable
dw 0xaa55 