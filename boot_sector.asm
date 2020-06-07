; Plus d'infos sur https://fr.wikipedia.org/wiki/INT_10H
mov ah, 0x0e ; tty mode (écrire un caractère télétype)
mov al, 'B' ; On met B dans l'octet de poids faible de EAX
int 0x10 ; On fait une interruption logicielle qui va afficher le caractère car on est en mode 0x0e
mov al, 'i'
int 0x10
mov al, 'e'
int 0x10
mov al, 'n'
int 0x10
mov al, 'v'
int 0x10
mov al, 'e'
int 0x10
mov al, 'n'
int 0x10
mov al, 'u'
int 0x10
mov al, 'e'
int 0x10
mov al, ' '
int 0x10
mov al, 's'
int 0x10
mov al, 'u'
int 0x10
mov al, 'r'
int 0x10
mov al, ' '
int 0x10
mov al, 'N'
int 0x10
mov al, 'i'
int 0x10
mov al, 'c'
int 0x10
mov al, 'O'
int 0x10
mov al, 'S'
int 0x10
mov al, ' '
int 0x10
mov al, '!'
int 0x10

; Boucle infinie
loop:
	jmp loop

; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre agic afin que ce soit bootable
dw 0xaa55 
