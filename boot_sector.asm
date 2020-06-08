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
mov al, ' '
int 0x10


; Lecture du secret
; On ajoute l'offset 0x7c00 (début de l'adresse du boot sector) et l'adresse du secret dans le fichier
; On utilise bx comme registre intermédiaire car il est impossible d'avoir un registre en destination et source d'une commande
; Note: en assembleur, [] donne la valeur dans le registre
mov bx, secret_data ; On met l'adresse de secret_data dans bx
add bx, 0x7c00 ; On ajoute 0x7c00 (adresse du boot sector)
mov al, [bx] ; On met [0x7c00 + secret_data] dans al (on a donc la valeur de secret_data)
int 0x10

mov bx, other_secret
add bx, 0x7c00
mov al, [bx]
int 0x10

; Un secret auquel on peut accéder dans la mémoire
secret_data:
    db "A"
    
; Un autre secret
other_secret:
    db "B"

mov al, ' '
int 0x10


; Usage de la pile. Rappel: l'adresse de la pile est decroissante
mov bp, 0x8000 ; On se place dans la mémoire libre
mov sp, bp ; On met bp (=0x8000) dans sp qui est le pointeur de la pile

push 'A' ; Adresse: 0x7ffe
push 'B' ; Adresse: 0x7ffc
push 'C' ; On met ces 3 valeurs sur la pile. Adresse: 0x7ffa

; pop permet de pop des mots, c'est à dire 16 bits. On pop donc dans bx et on déplace les octets de poids faible dans al pour le print
pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10
; Maintenant en 0x8000, les données ne valent plus rien de spécial


; Boucle infinie
jmp $

; On ajoute 510 - taille du code précédent "0"
times 510-($-$$) db 0

; Nombre agic afin que ce soit bootable
dw 0xaa55 
