C_SOURCES_32 = $(shell find . -name "*.c")
HEADERS = $(shell find . -name "*.h")
OBJ = $(C_SOURCES_32:.c=.o cpu/interrupts/interrupt.o kernel/gdt.o kernel/multiboot2.o) $(C_SOURCES_16:.c16=.o)

CC = gcc
GDB = gdb
CFLAGS = -Wall -Werror -Wextra -g -nostdlib -nostdinc -fno-pie -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
LD = ld
NASM = nasm

OS_IMG = OS.img

nicos.bin: $(OBJ)
	$(LD) -m elf_i386 -s -o $@ -T script.ld $^

nicos.elf: $(OBJ)
	$(LD) -m elf_i386 -o $@ -T script.ld $^

run: nicos.bin
	mcopy -i $(OS_IMG) -no nicos.bin ::/boot &
	qemu-system-i386 -m 1G -drive id=disk,file=$(OS_IMG),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0

debug: nicos.bin nicos.elf
	mcopy -i $(OS_IMG) -no nicos.bin ::/boot &
	qemu-system-i386 -m 1G -s -S -drive id=disk,file=$(OS_IMG),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0 &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file nicos.elf"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -m32 -c $< -o $@

%.o: %.asm
	$(NASM) $< -f elf -o $@

%.bin: %.asm
	$(NASM) $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o kernel/*.bin drivers/*.o boot/*.o cpu/*.o cpu/interrupts/*.o libc/*.o
