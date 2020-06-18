C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
OBJ = $(C_SOURCES:.c=.o cpu/interrupt.o)

CC = gcc
GDB = gdb
CFLAGS = -Wall -Werror -g -nostdinc -fno-pie -m32 -ffreestanding
LD = ld
NASM = nasm

nicos.bin: boot/boot_sector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o $(OBJ)
	$(LD) -m elf_i386 -s -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o $(OBJ)
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

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
