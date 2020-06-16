[bits 32]
[extern main] ; Définit le point appelé. Doit avoir le même nom dans kernel.c. Utile pour que le linker trouve main dans l'autre fichier.
call main ; Appele main()
jmp $ ; boucle infinie
