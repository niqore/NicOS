C_SOURCES_32 = $(shell find . -name "*.c" ! -path "./cmds/*")
HEADERS = $(shell find . -name "*.h" ! -path "./cmds/*")
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

commands:
	cd cmds && make all && cd ..

run: nicos.bin commands
ifeq ("$(wildcard $(OS_IMG))","")
	echo "Disc image $(OS_IMG) doesn't exist. Check README.md in order to create it"
else
	mcopy -i $(OS_IMG) -no nicos.bin ::/boot
	mcopy -i $(OS_IMG) -no cmds/bin ::/
	qemu-system-i386 -m 1G -drive id=disk,file=$(OS_IMG),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0
endif

debug: nicos.bin nicos.elf commands
ifeq ("$(wildcard $(OS_IMG))","")
	echo "Disc image $(OS_IMG) doesn't exist. Check README.md in order to create it"
else
	mcopy -i $(OS_IMG) -no nicos.bin ::/boot
	mcopy -i $(OS_IMG) -no cmds/bin ::/
	qemu-system-i386 -m 1G -s -S -drive id=disk,file=$(OS_IMG),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0 &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file nicos.elf"
endif

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -m32 -c $< -o $@

%.o: %.asm
	$(NASM) $< -f elf -o $@

%.bin: %.asm
	$(NASM) $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o kernel/*.bin drivers/*.o boot/*.o cpu/*.o cpu/interrupts/*.o libc/*.o
	cd cmds && make clean && cd ..

clean-image:
	rm $(OS_IMG)