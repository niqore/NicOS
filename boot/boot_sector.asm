[org 0x7c00] ; Indique à l'assembleur que l'offset est celui du bootsector. Il va donc décaler toutes données du code par 0x7c00

mov bx, WELCOME
call print
call print_nl
call print_nl


mov bp, 0x8000 ; Définition de la pile
mov sp, bp

mov bx, 0x9000 ; es:bx = 0x0000:0x9000 = 0x09000
mov dh, 2 ; Lire 2 secteurs
call disk_load

mov dx, [0x9000] ; Premier mot du premier secteur
call print_hex

call print_nl

mov dx, [0x9000 + 512] ; Premier mot du deuxième secteur
call print_hex


; Boucle infinie
jmp $

%include "boot/boot_sector_print.asm"
%include "boot/boot_sector_print_hex.asm"
%include "boot/boot_sector_disk.asm"

WELCOME:
	db 'Bienvenue sur NicOS !', 0; Le 0 sert à terminer la chaine de caractère

; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre magic afin que ce soit bootable
dw 0xaa55 


times 256 dw 0xdada ; secteur 2 = 512 bytes
times 256 dw 0xface ; secteur 3 = 512 bytes