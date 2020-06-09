[bits 32] ; on utilise le mode protégé (32 bits)

; Constantes
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; La couleur de chaque caractère

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx] ; [eb] est l'adresse du caractère courant
	mov ah, WHITE_ON_BLACK

	cmp al, 0 ; On regarde si le caractère est \00
	je print_string_pm_done

	mov [edx], ax ; On met le caractère et l'attribut dans la mémoire vidéo
	add ebx, 1 ; Prochain caractère
	add edx, 2 ; Prochaine position dans la mémoire vidéo

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret