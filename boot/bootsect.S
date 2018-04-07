/*
 * Boot sector of motoko.
 */
#include "../include/mm.h"
#include "../include/control_regs.h"
#include "../include/desc.h"

/* 
 * This boot sector will be loaded at physical address 0x7c00 by BIOS.
 * CPU is now in REAL MODE with CS=0x0 IP=0x7c00.
 * Real Mode Addressing:
 * 	Physical address = Segment << 4 + Offset
 */
.code16
.global _start
_start:
	cli		// Clear IF to block interrupts

	/*
	 * Clear out segment registers
	 */
	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss


	/*
	 * Enable A20 through 8042
	 */
wait8042_1:
	inb $0x64, %al	// Read from 0x64 port from 8042 for status.
	testb 0x2, %al	// Busy or not?
	jnz wait8042_1
	movb $0xd1, %al
	outb %al, $0x64	// 8042 Command: Write Output Port.
			// Next byte written to port 0x60 is placed
			// in the 8042 output port.
wait8042_2:
	inb $0x64, %al	
	testb 0x2, %al
	jnz wait8042_2
	movb $0xdf, %al
	outb %al, $0x60

	lgdt gdtdesc
	SET_CR0(CR0_PE)

	/*
	 * long jump to reload CS, EIP and instructions
	 */
	ljmp $K_CODE, $_startpe

.code32
_startpe:
	movw $K_DATA, %ax// Reload DS, SS, ES
	movw %ax, %ds
	movw %ax, %ss
	movw %ax, %es
	xorl %eax, %eax	// Clear out FS, GS
	movw %ax, %fs
	movw %ax, %gs

	movl $_start, %esp
	call load_os	// Load OS now!

.p2align 2		// 2^2 byte alignment
gdt:
	NULL_DESC	// GDT must start with a NULL descriptor
	SEG_DESC(TYPE_X|TYPE_R, 0x0, 0xffffffff)
	SEG_DESC(TYPE_D|TYPE_W, 0x0, 0xffffffff)

gdtdesc:
	.word (gdtdesc - gdt - 1)
	.long gdt

