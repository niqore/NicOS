C_SOURCES = $(shell find . -name "*.c")
HEADERS = $(shell find . -name "*.h")
OBJ = $(C_SOURCES:.c=.o cpu/interrupts/interrupt.o kernel/switch_pm.o)

CC = gcc
GDB = gdb
CFLAGS = -Wall -Werror -Wextra -g -nostdlib -nostdinc -fno-pie -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
LD = ld
NASM = nasm

nicos.bin: boot/boot_sector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o $(OBJ)
	$(LD) -m elf_i386 -s -o $@ -T script.ld $^ --oformat binary

kernel.elf: boot/kernel_entry.o $(OBJ)
	$(LD) -m elf_i386 -o $@ -T script.ld $^

run: nicos.bin
	qemu-system-i386 -fda $<

debug: nicos.bin kernel.elf
	qemu-system-i386 -s -S -fda $< &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(NASM) $< -f elf -o $@

%.bin: %.asm
	$(NASM) $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o
