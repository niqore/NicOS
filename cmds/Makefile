CC = gcc
CFLAGS = -Wall -Werror -Wextra -nostdlib -nostdinc -fPIE -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wl,-emain

all: bin/ls

bin/ls: src/ls/main.c
	$(CC) $(CFLAGS) -I../libc/shared/ -m32 $^ -o $@

clean:
	rm -rf bin/*