bin/boot_sector: boot/*.asm
	nasm -f bin boot/boot_sector.asm -o bin/boot_sector.bin
