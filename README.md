 
# NicOS

## Bootsector

Le bootsector doit faire 512 octets et doit finir par 0xaa55 qui est un magic number.

## Real mode memory map

Voir https://wiki.osdev.org/Memory_Map_(x86)

### 0x0 à 0x3FF: Interrupt Vector Table (IVT)

Tableau qui associe les interrupt handlers au interrupt requests. Doit être protégée pendant le boot.
Voir: https://en.wikipedia.org/wiki/Interrupt_vector_table

### 0x400 à 0x4FF: BIOS Data Area (BDA)

Mémoire réservée pour le BIOS. Elle contient par exemple les ports IO, des buffers etc...
Liste détaillée: http://www.bioscentral.com/misc/bda.htm

### 0x500 à 0x7BFF: Mémoire libre

Utilisation libre

### 0x7C00 à 07DFF: Boot sector

Correspond au binaire du bootsector chargé

### 0x7E00 à 0x9FBFF: Mémoire libre

Utlisation libre

### 0x9FC00 à 0x9FFFF: Extended BIOS Data Area (EBDA)

Permet au BIOS de sauvegarder des paramètres, des ports etc... Doit être protégée pendant le boot.
L'adresse peut changer mais ne dépasse jamais 0xA0000 et 128KiB en taille.

### 0xA0000 à 0xBFFFF: Vidéo memory

### 0xC0000 à 0xFFFFF: BIOS

### au delà de 0x100000: Extended memory

0x100000 à 0xEFFFFF: RAM, utilisation libre (14 MiB)
0xF00000 à 0xFFFFFF: Mémoire hardware (1 MiB)
0x1000000 à ???: RAM, utilisation libre
0xC0000000 à 0xFFFFFFFF: Mémoire pour les devices (1 GiB)
0x100000000 et +: RAM (PAE/64bit), utilisation libre

## Notions de base

Jusqu'au commit "Stack usage" inclus, ce sont les notions de base. Ensuite le code est changé drasticallement.

## Différence entre mode 16 bits réel et mode 32 bits protégé

On boot au début en mode BIOS 16 bits. Ce mode est simple mais justement trop pour un OS. C'est pourquoi les processeurs x86 ont un mode 32 bits (mode protégé).
Les différences notables sont:
- Les registres sont étendus à 32 bits, ce sont les mêmes qu'en 16 bits sauf qu'on met un e devant (ex: eax) (en 64 bits le e devient un r)
- Il y a 2 nouveaux registres: fs et gs
- On peut accéder au offsets de mémoires 32 bits, c'est-à-dire jusqu'à 4GB
- Le CPU est plus sophistiqué: mémoire virtuelle, segmentation etc...
- Les interruptions sont plus sophistiquées
- Impossible d'utiliser le BIOS

Pour passer de 16 à 32 bits il faut mettre en place la GDT (Global Table Descriptor) (https://fr.wikipedia.org/wiki/Global_Descriptor_Table)
Il faut aussi créer ses propres drivers pour chaque composants de la machine (clavier, souris, disk...).

## VGA (Video Graphics Array)

On peut mettre l'écran en 2 modes: texte ou graphique.
Le VGA possède une zone mémoire, qui commence en général à 0xb8000.

### Mode texte

Toutes les cartes graphiques commencent en mode texte. Le mode texte est composé de 80x25 caractères.
Dans ce mode, il n'est pas nécéssaire de s'embêter avec chaque pixel car les caractères existent de base.
Un caractère est défini comme 2 octets, le premier est le code ASCII et le deuxième les attributs (couleurs, clignotement).
L'adresse d'un caractère est: `0xb8000 + 2 * (row * 80 + col)`

## GDT (Global Table Descriptor)

Cela permet de segmenter la mémoire. La GDT sert à implément une mémoire virtuelle et non plus physique. En réalité, elle sert à indexer la mémoire virtuelle vers la mémoire physique.
Pour plus de détails, voir le fichier boot/32bit-gdt.asm et https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf page 38.

## Passage du mode 16 bits au mode 32 bits

La première chose à faire est de désactiver les inerruptions avec `cli` (clear interrupt). Le CPU va ainsi ignorer les interruptions.
Ceci est très important car les interruptions sont manipulées différemment en 32 bits. Ainsi l'IVT n'est plus utile.

La deuxième chose à faire est de passer la GDT avec `lgdt [gdt_descriptor]`

Ensuite, il faut mettre le premier bit du registre de contrôle du CPU, cr0, à 1.
Une fois que ceci est fait, le CPU est en mode 32 bits.
Un problème est qu'il peut rester des instructions 16 bits dans le pipeline, qui seront exécutées en 32 bits. Il faut donc flush le pipeline.