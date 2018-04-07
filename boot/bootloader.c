#include "../include/boot.h"
#include "../include/elf.h"
#include "../include/mm.h"

#define SECTSIZE 512 // Always be 512 byte

void wait_device()
{
	// Wait until BSY == 0 and RDY == 1
	// 0x3F6 is alternate status
	while ((inb(0x3F6) & 0xC0) != 0x40)
		;
}

/*
 * Read s_cnt sectors to dst
 */
void read_sectors(void *dst, unsigned int lba, unsigned char s_cnt)
{
	wait_device();

	outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
	outb(0x1F2, s_cnt);
	outb(0x1F3, lba & 0xFF);
	outb(0x1F4, (lba >> 8) & 0xFF);
	outb(0x1F5, (lba >> 16) & 0xFF);
	outb(0x1F7, 0x20); // Read sectors

	wait_device();
	insl(0x1F0, dst, SECTSIZE / 4 * s_cnt);
}

/*
 * Load Operating System
 */
void load_os()
{
	Elf32_Ehdr *kern_header;
	Elf32_Phdr *phdr;
	int idx_seg;
	int num_seg;
	unsigned int lba;
	void *dst;
	unsigned char s_cnt;
	void (*entry)();

	// Load second sector containing ELF header to temporary location.
	read_sectors((void *)0x10000, 0x1, 0x1);

	kern_header = (Elf32_Ehdr *)0x10000;
	num_seg = kern_header->e_phnum;
	phdr	= (Elf32_Phdr *)(kern_header + kern_header->e_phoff);

	// Load Program Segments
	for (idx_seg = 0; idx_seg < num_seg; idx_seg++, phdr++) {
		lba = phdr->p_offset / SECTSIZE + 1; // On which sector.

		s_cnt = (phdr->p_offset + phdr->p_filesz + SECTSIZE) / SECTSIZE
			- phdr->p_offset / SECTSIZE;

		dst = (void *)(phdr->p_paddr - phdr->p_offset % SECTSIZE);

		read_sectors(dst, lba, s_cnt);

		if (phdr->p_memsz > phdr->p_filesz)
			stosb((void *)(phdr->p_paddr + phdr->p_filesz), 
				0x0, phdr->p_memsz - phdr->p_filesz);
	}

	// Boot kernel
	entry = (void (*)())kern_header->e_entry;
	entry();
}
