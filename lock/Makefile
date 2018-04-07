SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
#
# Tool chain
#
CC = gcc
AS = as
LD = ld

#
# Flags
#
CFLAGS = -fno-pic -nostdinc -fno-stack-protector -fno-omit-frame-pointer
CFLAGS+= -static -fno-builtin -O -m32
LDFLAGS = -m elf_i386 -N
ASFLAGS = -m32

.PHONY: all

all:$(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	-rm *.o
