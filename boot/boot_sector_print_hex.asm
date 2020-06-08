print_hex:
	pusha
	mov cx, 0 ; l'index

; On récupère le dernier caractère de dx et on le converti en ASCII
; '0' (en ASCII 0x30) à '9' (0x39), on ajoute donc 0x30 à l'octet
; 'A' (0x41) à 'F' (0x46), on ajoute donc 0x40
hex_loop:
	cmp cx, 4 ; On fait la boucle 4 fois
	je end

	; Conversion du dernier caractère en ASCII
	mov ax, dx ; Registre de travail
	and ax, 0x000f ; On enlève les 3 premiers chiffres (ex: 0x1234 devient 0x0004)
	add al, 0x30 ; On ajoute 0x30 pour se placer dans la table ascii

	cmp al, 0x39 ; Si > 9, on ajoute 7 pour réprésenter 'A' à 'F'
	jle step2
	add al, 7 ; 'A' est l'ascii 65 (0x41) en décimal et non 58 (0x39 + 1) donc 65-58=7

step2:
	mov bx, HEX_OUT + 5 ; On se place au niveau du dernier caractère de HEX_OUT
	sub bx, cx ; Revient à HEX_OUT + 5 - index du char
	mov [bx], al ; On met le caractère trouvé dans loop à la bonne place dans HEX_OUT
	ror dx, 4 ; On prépare le prochain caractère en faisant tourner les bits (ex: 0x1234 devient 0x4123)

	add cx, 1 ; Incrémentation de la boucle
	jmp hex_loop

end:
	mov bx, HEX_OUT
	call print

	popa
	ret

HEX_OUT:
	db '0x0000', 0 ; Place réservée pour la chaine finale