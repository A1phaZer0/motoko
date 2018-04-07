/*
 * The very beginning of the kernel image
 */
#include "include/mm.h"
#include "include/control_regs.h"

.global _k_start
_k_start = EXT_MEM

.global _entry
_entry:
	/*
	 * Initialize Page Directory
	 */
	movl $(PDINX(KERN_BASE)), %eax
	movl $(L2P(kern_pgdir)), %ebx
	movl $(L2P(kern_pgtbl)), (%ebx, %eax, 4)
	/*
	 * Setup PDBR (CR3)
	 */
	movl $(L2P(kern_pgdir)), %eax
	movl %eax, %cr3

	/*
	 * Initialize Page Table for kernel
	 */
	pushfl

	movl $0, %eax
	orl $(PTE_P|PTE_W), %eax	// Page starts at 0x00000000
	movl $PTE_NUM, %ecx
	movl $(L2P(kern_pgtbl)), %edx	// Page table address
INIT_PT:				// Initialize kernel page table
	movl %eax, (%edx)
	add $PG_SIZE, %eax		// Physical address of next page
	add $4, %edx			// Next page table entry
	sub $1, %ecx
	cmp $0, %ecx			// If ecx == 0	
	jnz INIT_PT

	popfl
	/*
	 * Enable Paging
	 */
	SET_CR0(CR0_PG)

	/*
	 * Setup kernel stack
	 */
	movl $(kern_stack + KSTACKSIZE), %esp

	/*
	 * Indirect jump to avoid pc-relative addressing.
	 */
	movl $main, %eax
	jmp *%eax



	.comm kern_stack, KSTACKSIZE	// 4KB kernel stack

	

