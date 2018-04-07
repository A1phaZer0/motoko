/*
 * Macros for manipulating control registers
 */

/*
 * Flags for CR0
 */
#define CR0_PE		0x00000001	// Protected Mode Enable
#define CR0_PG		0x80000000	// Paging
#define CR0_MP		0x00000002	// Math Present
#define CR0_EM		0x00000004	// EMulation
#define CR0_TS		0x00000008	// Task Switched
#define CR0_ET		0x00000010	// Extension Type
#define CR0_NE		0x00000020	// Numric Error
#define CR0_WP		0x00010000	// Write Protect
#define CR0_AM		0x00040000	// Alignment Mask

/*
 * Flags for EFLAGS
 */
#define EFLAGS_CF	0x00000001	// Carry
#define EFLAGS_PF	0x00000004	// Parity
#define EFLAGS_AF	0x00000010	// Adjust
#define EFLAGS_ZF	0x00000040	// Zero
#define EFLAGS_SF	0x00000080	// Signedness
#define EFLAGS_TF	0x00000100	// Trap
#define EFLAGS_IF	0x00000200	// Interrupt
#define EFLAGS_DF	0x00000400	// Direction
#define EFLAGS_OF	0x00000800	// Overflow
#define EFLAGS_NT	0x00004000	// Nested task
#define EFLAGS_RF	0x00010000	// Resume
#define EFLAGS_VM	0x00020000	// Virtual Machine
#define EFLAGS_IOPL_K  	0xFFFFCFFF	// IOPL to ring 0 (kernel)
#define EFLAGS_IOPL_U  	0x00003000	// IOPL to ring 3 (user)

#define SET_CR0(flag)	\
	movl %cr0, %eax;	\
	orl $(flag), %eax;	\
	movl %eax, %cr0

/*
 * Set ring 0 IOPL
 */
#define SET_IOPL(iopl)	\
	movl %cr0, %eax;	\
	andl $(iopl), %eax;	\
	movl %eax, %cr0

