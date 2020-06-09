[bits 16]
switch_to_pm:
	cli ; Désactivation des interruptions
	lgdt [gdt_descriptor] ; Chargement de la GDT
	mov eax, cr0 ; On récupère cr0 pour travailler dessus
	or eax, 0x1 ; On met le premier bit à 1
	mov cr0, eax ; On met la nouvelle valeur dans cr0
	jmp CODE_SEG:init_pm ; On saute loin dans le code pour flush le pipeline
	
[bits 32]
init_pm:
	mov ax, DATA_SEG ; On update les registres des segments
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000 ; On met à jour la pile dans un endroit libre
	mov esp, ebp ; On place ebp en haut de la pile
	
	call BEGIN_PM