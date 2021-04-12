# NicOS

NicOS est un noyau de système d'exploitation créé en assembleur et C. C'est un projet réalisé dans le cadre scolaire.

## Compilation et création de l'image disque (étape non nécessaire si vous avez déjà une image précompilée)

### Outils nécessaires

- [gcc](https://gcc.gnu.org/)
- [binutils](https://www.gnu.org/software/binutils/) pouvant lier au format elf_i386 (voir https://wiki.osdev.org/GCC_Cross-Compiler)
- [make](https://www.gnu.org/software/make/)
- [nasm](https://www.nasm.us/)
- [mtools](https://www.gnu.org/software/mtools/)
- [grub-install](https://www.gnu.org/software/grub/) (attention à bien avoir la version 2)

### Compilation du code

```
make
```

### Création de l'image disque

Dans les commandes suivantes, le mode *sudo* peut être nécessaire.

Il faut d'abord créer une image vierge avec la commande suivante. count peut-être changé avec la valeur en octets voulue
```
dd if=/dev/zero of=OS.img bs=1k count=100000
```

Il faut ensuite formatter l'image en vFAT
```
/usr/sbin/mkfs.vfat -F 32 OS.img
```

Ensuite, il faut monter l'image disque avec la commande suivante. Notez que loop0 peut-être changé en loop1, 2, 3...
```
mkdir /mnt/nicos
losetup /dev/loop0 OS.img
mount /dev/loop0 /mnt/nicos
```

Enfin, il faut installer grub avec:
```
/usr/sbin/grub-install --root-directory=/mnt/nicos --no-floppy --recheck --force /dev/loop0
```

Il faut ensuite démonter l'image et supprimer le dossier temporaire
```
umount /dev/loop0
rmdir /mnt/nicos
```

Enfin, il faut copier le noyau, le fichier de configuration et les commandes avec les commandes:
```
mcopy -i OS.img -no nicos.bin ::/boot
mcopy -i OS.img -no grub/grub.cfg ::/boot/grub
mcopy -i OS.img -no cmds/bin ::/
```

## Lancement du système d'exploitation avec QEMU

Il faut avoir installé [QEMU](https://www.qemu.org/)

Ensuite il faut exécuter la commande suivante avec le fichier OS.img dans le dossier courant:
```
qemu-system-i386 -m 1G -drive id=disk,format=raw,file=OS.img,if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0
```
