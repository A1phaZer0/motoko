/*
 * Linker script for kernel
 */

/*OUTPUT_FORMAT=("elf32-i386", "elf32-i386", "elf32-i386")*/
ENTRY(_k_start)


SECTIONS
{
	. = 0x80100000;

	/* 
	 * Set .text segment starts at 
	 * VMA = 0x80100000
	 * LMA = 0x100000
	 */
	.text : AT(0x100000) {
		*(.text .stub .text.* .gnu.linkonce.t.*)
	}

	/*
	 * Address of the byte after .text segment
	 */
	PROVIDE(etext = .); 

	.rodata : {
		*(.rodata .rodata.* .gnu.likonce.r.*)
	}

	/*
	 * Paged aligned VMA
	 */
	. = ALIGN(0x1000);

	PROVIDE(bdata = .);
	.data : {
		*(.data)
	}

	PROVIDE(edata = .);

	.bss : {
		*(.bss)
	}

	PROVIDE(end = .);

	/DISCARD/ : {
		*(.eh_frame .note.GNU-stack)
	}
}
