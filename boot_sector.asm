[org 0x7c00] ; Indique à l'assembleur que l'offset est celui du bootsector

mov bx, WELCOME
call print
call print_nl

mov dx, 0x12fe
call print_hex

; Boucle infinie
jmp $

%include "boot_sector_print.asm"
%include "boot_sector_print_hex.asm"

WELCOME:
	db 'Bienvenue sur NicOS !', 0; Le 0 sert à terminer la chaine de caractère

; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre agic afin que ce soit bootable
dw 0xaa55 
