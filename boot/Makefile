#
# Tool chain
#
CC = gcc
AS = as
LD = ld
OBJCOPY = objcopy

#
# Flags
#
CFLAGS = -fno-pic -nostdinc -fno-stack-protector -fno-omit-frame-pointer
CFLAGS+= -static -fno-builtin -O -m32
LDFLAGS = -m elf_i386 -N
ASFLAGS = -m32

bootblock:bootsect.S bootloader.c
	$(CC) $(CFLAGS) -c bootloader.c
	$(CC) $(CFLAGS) -c bootsect.S
	$(LD) $(LDFLAGS) -Ttext 0x7c00 -e _start -o bootblock bootsect.o bootloader.o
	$(OBJCOPY) -S -O binary -j .text bootblock boot
	./create_sector.py

.PHONY: clean
clean:
	-rm *.o bootblock boot
