 
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

## Interruptions en 32 bits

### IDT (Interrupt Descriptor Table)

L'IDT est exclusif au mode 32 bits. C'est une table indiquant où se trouvent les ISR (voir partie suivante). Les entrées de l'IDT sont appelées gates. Les gates contiennent eux-mêmes les Interrupt Gates, Task Gates et Trap Gates.

#### Emplacement et taille

L'adresse et la taille de l'IDT sont sauvegardées dans le registre _IDTR_. Pour l'affecter, il faut utiliser les commandes assembleur lidt ou sidt.
Les 16 premiers bits du registre représente la taille (entre 256 et 4096) et les 48 bits suivants l'adresse de la table.

#### Gates

Un gate fait 8 octets. Les bits sont les suivants:
- 2 octets: bits 0 à 15 de l'offset. L'offset est le point d'entrée de l'ISR (adresse)
- 2 octets: le _code segment selector_ dans GDT ou LDT. Doit être l'adresse valide de la GDT
- 1 octet: doit valoir 0 (unitilisé)
- 1 octet: types et attributs:
	- bits 0 à 3: gate type (0b0101 = 32-bit task gate, 0b0110 = 16-bit interrupt gates, 0b0111 = 16 trap, 0b1110 = 32 interrupt, 0b1111 = 32 trap)
	- bit 4: 0 pour interrupt et trap gate
	- bit 5 et 6: Niveau de privilège
	- bit 7: 0 pour les interruptions inutilisées

	- bit 0: 0 pour les interruptions inutilisées
	- bit 1 et 2: Niveau de privilège
	- bit 3: 0 pour interrupt et trap gate
	- bits 4 à 7: gate type (0b0101 = 32-bit task gate, 0b0110 = 16-bit interrupt gates, 0b0111 = 16 trap, 0b1110 = 32 interrupt, 0b1111 = 32 trap)
- 2 octets: bit 16 à 31 de l'offset

Types de gates:
- Interrupt gate: Permet de spécifier un ISR (voir après). La valeur courante utilisée pour le gate est 0x8E pour le 32 bits
- Trap gate: La seule différence avec les interrupt gates est que les interrupt gates restaurent automatiquement l'état du CPU au _iret_. Valeur courante: 0x8F
- Task gate: A task gate in the IDT references a TSS descriptor in the GDT. (voir https://wiki.osdev.org/Interrupt_Descriptor_Table#I386_Task_Gate)

### ISR (Interrupt Service Routines)

L'ISR est executé à chaque fois que le CPU récupère une exception.
Les 32 première sont disponibles sur https://wiki.osdev.org/Exceptions

### PIC 8259 (Programmable Interrupt Controller)

Le PIC est un hardware de l'architecture x86. Il permet de gérer les interruptions en dehors du CPU et donc de ne pas perdre de temps et d'éviter que le CPU check en boucle si un événement à lieu (clavier...).

#### Fonctionnement global

Certains périphériques envoient des signaux au PIC sur une ligne d'interruption (IRQ ligne). Par exemple, le calvier, quand une touche est frappée, envoie un IRQ1 sur le PIC. Un PIC à 8 IRQ line.

Les ancien système n'utilisaient qu'un seul PIC, mais les plus récents en utilisent 2 en cascade. Ainsi, il y a une possibilité de 15 interruptions (et non 16, car le PIC2 doit pouvoir envoyer le signal sur l'IRQ2 du PIC1).

#### Fonctionnement interne

Quand un PIC reçoit un signal, il met un bit à 1 qui permet de dire que l'interruption doit être traitée. Si un channel n'est pas masqué et que le bit est à 0, l'interruption est alors traitée. L'interruption est transmise du PIC2 au PIC1 (esclave à maitre) puis du PIC1 au CPU.

#### IO-APIC (I/O Advanced Programmable Interrupt Controller)

Aujourd'hui, ce sont les APIC qui sont utilisés pour les processeurs multi coeurs. Chaque processeur contient un local APIC et il y a des I/O APIC répartis dans le système par exemple au niveau des chipsets ou bridges PCI. Un I/O APIC possède 24 entrées a une base qui définit ses numéros d'interruption (il génèrera alors des interruptions entre base et base + 23).
Voir Intel OS Dev.pdf chapitre 10.

## Create OS Fat32 Disk Image

```
dd if=/dev/zero of=OS.img bs=1k count=100000
sudo /usr/sbin/mkfs.vfat -F 32 OS.img
sudo losetup /dev/loop0 OS.img
sudo mount /dev/loop0 /mnt
sudo grub-install --root-directory=/mnt --no-floppy --recheck --force /dev/loop0
```

Pour copier l'OS
```
mcopy -i OS.img nicos.bin ::/boot
```

Pour lancer avec Qemu
```
qemu-system-i386 -m 1G -hda OS.img
```