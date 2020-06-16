LD = ld
NASM = nasm
CC = gcc

BIN = bin
BUILD = build

all: run

$(BUILD)/kernel.bin: $(BUILD)/kernel_entry.o $(BUILD)/kernel.o
	$(LD) -m elf_i386 -s -o $@ -Ttext 0x1000 $^ --oformat binary
	
$(BUILD)/kernel_entry.o: boot/kernel_entry.asm
	$(NASM) $< -f elf -o $@
	
$(BUILD)/kernel.o: kernel/kernel.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@
	
$(BUILD)/boot_sector.bin: boot/boot_sector.asm
	$(NASM) $< -f bin -o $@
	
$(BIN)/nicos.bin: $(BUILD)/boot_sector.bin $(BUILD)/kernel.bin
	cat $^ > $@
	
run: $(BIN)/nicos.bin
	qemu-system-i386 -fda $<
	
clean:
	rm $(BUILD)/*.bin $(BUILD)/*.o $(BIN)/*.bin
