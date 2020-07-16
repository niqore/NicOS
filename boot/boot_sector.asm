[org 0x7c00] ; Indique à l'assembleur que l'offset est celui du bootsector. Il va donc décaler toutes données du code par 0x7c00

;jmp 0x00:start ; On met cs à 0
;nop

; FAT 32 header
db 0xEB,0x58,0x90
db 'nicosfat'
db 0x00,0x02,0x01,0x20,0x00,0x02
times 4 db 0
db 0xF8,0x00,0x00,0x20,0x00,0x40
times 5 db 0
db 0x40,0x0D,0x03,0x00,0x03,0x06
times 6 db 0
db 0x02
times 3 db 0
db 0x01,0x00,0x06
times 13 db 0
db 0x80,0x00,0x29,0xA2,0x0E,0x7F,0x8C
db 'NO NAME    '
db 'FAT32   '


start:
  xor ax, ax ; On met ax à 0
  mov es, ax
  mov ds, ax
  mov ss, ax

KERNEL_OFFSET equ 0x1000 ; Définition de l'offset du kernel

mov [BOOT_DRIVE], dl ; Le BIOS enregistre le périphérique de démarrage dans dl donc on l'enregistre pour l'utiliser plus tard si besoin
mov bp, 0x7c00 ; pile
mov sp, bp

call load_kernel

call KERNEL_OFFSET ; Appelle de la fonction d'entrée dans le kernel

; Boucle infinie. Ne sera jamais exécutée
jmp $

%include "boot/boot_sector_print.asm"
%include "boot/boot_sector_print_hex.asm"
%include "boot/boot_sector_disk.asm"

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET ; Lecture depuis le disque et enregistrement en 0x1000
	mov dh, 50
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

BOOT_DRIVE db 0

; Bootsector
; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre magic afin que ce soit bootable
dw 0xaa55 
