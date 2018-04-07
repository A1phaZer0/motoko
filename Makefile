#
# Makefile for building demo
#
DIRS = mm task lock
BOOT_DIR = boot
SRCS = $(foreach subdir, $(DIRS), $(wildcard $(subdir)/*.c))
OBJS = $(patsubst %.c, %.o, $(SRCS))
OBJS+= main.o

#
# Tool chain
#
CC = gcc
AS = as
LD = ld

#
# Tool chain flags
#
CFLAGS = -fno-pic -nostdinc -fno-stack-protector -fno-omit-frame-pointer
CFLAGS+= -static -fno-builtin -O0 -m32
LDFLAGS = -m elf_i386
ASFLAGS = -m32

#
# Create kernel image
#
kernel.img:kernel
	dd if=/dev/zero of=kernel.img count=10000
	dd if=boot/boot of=kernel.img conv=notrunc
	dd if=kernel of=kernel.img seek=1 conv=notrunc

#
# Link kernel with entry.o
#
kernel:all
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel entry.o $(OBJS)

#
# Compile all targets
#
.PHONY: all
all:
	$(MAKE) -C boot
	$(MAKE) all -C mm
	$(MAKE) all -C lock
	$(CC) $(CFLAGS) -c entry.S
	$(CC) $(CFLAGS) -c main.c

.PHONY: clean
clean:
	$(MAKE) clean -C boot
	$(MAKE) clean -C mm
	$(MAKE) clean -C lock
	-rm kernel kernel.img entry.o main.o
