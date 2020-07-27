[bits 32]
gdt_start: ; Les labels doivent être présent pour calculer la taille et les sauts
 	dd 0x0 ; dd = Define double word (4 octets) (db = define byte 1o, dw = define word 2o)
 	dd 0x0 ; La GDT commence par 8 octets à 0

; Voir https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf page 34
; base = 0x00000000, length = 0xfffff
gdt_code:
	dw 0xffff ; longueur du segment, bits 0 à 15
	dw 0x0 ; base du segment (=adresse de base), bits 0 à 15
	db 0x0 ; base du segment, bits 16 à 23
	db 10011010b ; flags de type (8 bits)
	; bit 1: segment présent dans la mémoire (utilisé pour la mémoire virtuelle)
	; bit 2: privilege
	; bit 3: privilege ring
	; bit 4: type de descripteur (1 pour du code ou segment de données, 0 pour les traps)
	; bit 5: type code
	; bit 6: conforming, 0 veut dire qu'un segment avec un privilege inférieur ne peut pas lire ce segment (protection de mémoire)
	; bit 7: readible
	; bit 8: accessed (debugging)
	db 11001111b ; flags (4 bits) + longueur du seglement, bits 16 à 19
	; bit 1: granularité, si 1, multiplie la limite par 4K (16*16*16) (revient à décaler de 3 digits vers la gauche, ex 0xfffff devient 0xfffff000) pour pouvoir atteindre des adresses plus grandes que 4Gb
	; bit 2: default operation size (0 = 16 bits segments, 1 = 32 bits segment)
	; bit 3: 64 bit code segment, non utilisé sur des processeur 32 bits
	; bit 4: available for use by system software (debugging)
	; bits 5 à 8: longueur du segment (suite)
	db 0x0 ; base du segment, bits 24-31
	
gdt_data:
	; Même code que précédemment sauf pour les flags du type:
	; bit 1: code, bit 2: expand down, bit 3: writable, bit 4: accessed
	dw 0xffff ; longueur du segment
	dw 0x0 ; base du segment (0-15)
	db 0x0 ; base du segment (16-23)
	db 10010010b ; flag de type
	db 11001111b ; flags (4 bits) + longueur du segment bits 16 à 19
	db 0x0 ; base (bits 24-31)
	
gdt_end: ; Ce label vide permet de calculer dans le label suivant la taille

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; Taille de la GDT
	dd gdt_start ; Adresse de départ de la GDT
	
; Définitions de constantes pour le decripteur des offsets des segments, qui sont ce que doivent contenir les registres des segments en mode protégé.
; Par exemple, quand on met ds = 0x10 en PM (protected mode), le CPU sait qu'on veut le segment d'offset 0x10 dans la GDT, qui est dans notre cas le segment DATA
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

global set_gdt
set_gdt:
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0 ; On récupère cr0 pour travailler dessus
	or eax, 0x1 ; On met le premier bit à 1
	mov cr0, eax ; On met la nouvelle valeur dans cr0
	jmp CODE_SEG:init_pm

init_pm:
	mov ax, DATA_SEG ; On update les registres des segments
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	ret