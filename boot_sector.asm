; Boucle infinie
loop:
	jmp loop

; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre agic afin que ce soit bootable
dw 0xaa55 
