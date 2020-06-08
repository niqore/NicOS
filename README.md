 
# NicOS

## Bootsector

Le bootsector doit faire 512 octets et doit finir par 0xaa55 qui est un magic number.

## Real mode memory map

Voir https://wiki.osdev.org/Memory_Map_(x86)

### 0x0 à 0x3FF: Interrupt Vector Table

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