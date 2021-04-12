CC = gcc
CFLAGS = -Wall -Werror -Wextra -nostdlib -nostdinc -fPIE -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wl,-emain

all: bin/ls bin/cat bin/snake bin/lspci bin/raminfo

bin/snake: src/snake/main.c
	$(CC) $(CFLAGS) -I../libc/shared/ -m32 $^ -o $@

bin/ls: src/ls/main.c
	$(CC) $(CFLAGS) -I../libc/shared/ -m32 $^ -o $@

bin/cat: src/cat/main.c
	$(CC) $(CFLAGS) -I../libc/shared/ -m32 $^ -o $@

bin/lspci: src/lspci/main.c
	$(CC) $(CFLAGS) -I../libc/shared/ -m32 $^ -o $@

bin/raminfo: src/raminfo/main.c
	$(CC) $(CFLAGS) -I../libc/shared/ -m32 $^ -o $@

clean:
	rm -rf bin/*