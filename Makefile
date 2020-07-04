C_SOURCES_32 = $(shell find . -name "*.c")
C_SOURCES_16 = $(shell find . -name "*.c16")
HEADERS = $(shell find . -name "*.h")
OBJ = $(C_SOURCES_32:.c=.o cpu/interrupts/interrupt.o kernel/switch_pm.o kernel/memory_map.o) $(C_SOURCES_16:.c16=.o)

CC = gcc
GDB = gdb
CFLAGS = -Wall -Werror -Wextra -g -nostdlib -nostdinc -fno-pie -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
LD = ld
NASM = nasm

nicos.bin: boot/boot_sector.bin kernel.bin
	cat $^ > $@

kernel.bin: $(OBJ)
	$(LD) -m elf_i386 -s -o $@ -T script.ld $^ --oformat binary

kernel.elf: $(OBJ)
	$(LD) -m elf_i386 -o $@ -T script.ld $^

run: nicos.bin
	qemu-system-i386 -m 1G -fda $<

debug: nicos.bin kernel.elf
	qemu-system-i386 -m 1G -s -S -fda $< &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -m32 -c $< -o $@

%.o: %.c16 $(HEADERS)
	$(CC) $(CFLAGS) -m16 -x c -c $< -o $@

%.o: %.asm
	$(NASM) $< -f elf -o $@

%.bin: %.asm
	$(NASM) $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o
