 disk_load:
 	pusha
 	push dx ; Sauvegarde des registres pour pouvoir les remettre bien à la fin

 	mov ah, 0x02 ; Read sector from drive avec INT 13H (https://en.wikipedia.org/wiki/INT_13H)
 	mov al, dh ; al correspond au nombre de secteur à lire
 	mov cl, 0x02 ; cl = numéro du secteur. 0x00 est le boot sector et 0x02 est le premier secteur disponible

 	mov ch, 0x00 ; cylindre du disque dur
 	mov dh, 0x00 ; tête du disque dur (https://fr.wikipedia.org/wiki/Cylindre/T%C3%AAte/Secteur)

 	int 0x13 ; Interruption BIOS
 	jc disk_error ; Si il y a une erreur. Si c'est le cas, l'indicateur de retenue (carry bit) est à 1

 	pop dx
 	cmp al, dh ; le BIOS met 'al' au nombre de secteur lu, on compare donc pour voir s'il y a une erreur
 	jne sectors_error
 	popa
 	ret

 disk_error:
 	mov bx, DISK_ERROR
 	call print
 	call print_nl
 	mov dh, ah ; ah = code d'erreur, dl = disque qui a envoyé l'erreur
 	call print_hex
 	jmp disk_loop

 sectors_error:
 	mov bx, SECTORS_ERROR
 	call print

 disk_loop:
 	jmp $


DISK_ERROR: db "Erreur de lecture du disque", 0
SECTORS_ERROR: db "Nombre incorrect de secteurs lus", 0