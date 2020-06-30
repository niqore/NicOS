%include "kernel/gdt.asm"

[bits 16]
global switch_to_pm
switch_to_pm:
	cli ; Désactivation des interruptions
	lgdt [gdt_descriptor] ; Chargement de la GDT
	mov eax, cr0 ; On récupère cr0 pour travailler dessus
	or eax, 0x1 ; On met le premier bit à 1
	mov cr0, eax ; On met la nouvelle valeur dans cr0
	jmp CODE_SEG:init_pm ; On saute loin dans le code pour flush le pipeline
	
[bits 32]
[extern main]
init_pm:
	mov ax, DATA_SEG ; On update les registres des segments
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call main