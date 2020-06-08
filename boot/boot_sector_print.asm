 print:
 	pusha ; Ceci met, dans l'ordre, les registres AX, CX, DX, BX, SP, BP, SI, DI sur la pile

start:
	mov al, [bx] ; On met dans al l'adresse de bx qui contient le premier caractère

	cmp al, 0 ; Si on est à 0 c'est qu'on est à la fin de la chaine
	je done

	mov ah, 0x0e; tty mode
	int 0x10

	add bx, 1
	jmp start

done:
	popa
	ret

print_nl:
	pusha

	mov ah, 0x0e
	mov al, 0x0a
	int 0x10
	mov al, 0x0d
	int 0x10

	popa
	ret