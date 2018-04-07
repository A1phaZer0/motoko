/*
 * Macros for creating segment descriptor
 */

/*
 * CODE/DATA segment descriptor
 */
#define TYPE_A		0x1	// Accessed
#define TYPE_W		0x2	// Writable data segment
#define TYPE_E		0x4	// Data segment expand down
#define TYPE_D		0x0	// Data segment
#define TYPE_R		0x2	// Readable code segment
#define	TYPE_C		0x4	// Conforming code segment
#define TYPE_X		0x8	// Code segment
#define RING3_X		0xF0	// P=1 DPL=3 S=1, Code/Data segment on ring 3
#define RING0_X		0x90	// Code/Data segment on ring 0

/*
 * SYSTEM segment descriptor
 */
#define TYPE_LDT	0x2	// LDT segment
#define TYPE_AVL_TSS	0x9	// Available TSS
#define TYPE_BUSY_TSS	0xB	// Busy TSS
#define TYPE_INT_GATE	0xE	// Interrupt Gate
#define TYPE_TRAP_GATE	0xF	// Trap Gate
#define RING0_SYS	0x80	// System segment on ring 0, P=1 DPL=0 S=0

#define GR_BIG		0xC	// Set granularity to 4KB, 32 bit mode

/*
 * Create segment descriptor
 * limit >> 12 gives limit in 4K pages.
 */
#define SEG_DESC(type,base,limit)	\
	.word (((limit) >> 12) & 0xffff), ((base) & 0xffff);	\
	.byte (((base) >> 16) & 0xff), (type), 	\
		((GR_BIG << 4) | (((limit) >> 28) & 0xf)),	\
		(((base) >> 24) & 0xff)

/*
 * NULL Descriptor, the first descriptor in GDT
 */
#define NULL_DESC	\
	.word 0, 0;	\
	.byte 0, 0, 0, 0

