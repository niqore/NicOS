[org 0x7c00] ; Indique à l'assembleur que l'offset est celui du bootsector. Il va donc décaler toutes données du code par 0x7c00
KERNEL_OFFSET equ 0x1000 ; Définition de l'offset du kernel

mov [BOOT_DRIVE], dl ; Le BIOS enregistre le périphérique de démarrage dans dl donc on l'enregistre pour l'utiliser plus tard si besoin
mov bp, 0x9000 ; pile
mov sp, bp

mov bx, WELCOME
call print
call print_nl
call print_nl

mov bx, PROT_MODE_LOAD_MSG
call print
call print_nl

call load_kernel
call switch_to_pm

; Boucle infinie. Ne sera jamais exécutée
jmp $

%include "boot/boot_sector_print.asm"
%include "boot/32bit-gdt.asm"
%include "boot/32bit-print.asm"
%include "boot/32bit-switch.asm"
%include "boot/boot_sector_print_hex.asm"
%include "boot/boot_sector_disk.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print
	call print_nl

	mov bx, KERNEL_OFFSET ; Lecture depuis le disque et enregistrement en 0x1000
	mov dh, 2
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
BEGIN_PM: ; Point d'entrée en mode 32 bits
	mov ebx, PROT_MODE_ENTERED_MSG
	call print_string_pm
	call KERNEL_OFFSET ; Appelle de la fonction d'entrée dans le kernel
	jmp $

WELCOME:
	db 'Bienvenue sur NicOS !', 0; Le 0 sert à terminer la chaine de caractère
	
PROT_MODE_LOAD_MSG:
	db 'Passage en mode 32-bit protected...', 0
	
PROT_MODE_ENTERED_MSG:
	db 'Entree en mode 32-bit finie !', 0

MSG_LOAD_KERNEL:
	db 'Chargement du kernel', 0

BOOT_DRIVE db 0

; Bootsector
; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre magic afin que ce soit bootable
dw 0xaa55 