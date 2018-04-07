/*
 * Helper function for boot sector and boot loader
 */

/*
 * outb %al, %dx
 */
extern inline void outb(unsigned short port, unsigned char val)
{
	__asm__ __volatile__(
		"outb %0, %1"
		::"a"(val), "d"(port):);
}


/*
 * inb %dx, %al
 */
extern inline unsigned char inb(unsigned short port)
{
	unsigned char ret;
	__asm__ __volatile__ (
		"inb %1, %0"
		:"=a"(ret)
		:"d"(port):
		);
	return ret;
}



/*
 * repeatly do "insl"
 * read data from port to ES:EDI
 */
extern inline void insl(unsigned short port, void *pos, unsigned int count)
{
	__asm__ __volatile__(
		"cld\n\t"
		"rep insl"
		:"=D"(pos), "=c"(count)
		:"0"(pos), "1"(count), "d"(port)
		:"cc"
		);
}

/*
 * repeatly do "stosb"
 * save data in AL to ES:EDI
 */
extern inline void stosb(void *pos, unsigned char data, unsigned int count)
{
	__asm__ __volatile__ (
		"cld\n\t"
		"rep stosb"
		:"=D"(pos), "=c"(count)
		:"D"(pos), "c"(count), "a"(data)
		:"cc", "memory"
		);
}
