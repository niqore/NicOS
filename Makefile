bin/boot_sector: boot_sector.asm
	nasm -f bin boot_sector.asm -o bin/boot_sector.bin 
