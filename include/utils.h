/*
 * Helper functions
 */

/*
 * outb %al, %dx
 */
static inline void outb(unsigned short port, unsigned char val)
{
	__asm__ __volatile__(
		"outb %0, %1"
		::"a"(val), "d"(port):);
}

/*
 * outw %ax, %dx
 */
static inline void outw(unsigned short port, unsigned short val)
{
	__asm__ __volatile__(
		"outw %0, %1"
		::"a"(val), "d"(port):);
}

/*
 * outl %eax, %dx
 */
static inline void outl(unsigned short port, unsigned int val)
{
	__asm__ __volatile__(
		"outw %0, %1"
		::"a"(val), "d"(port):);
}

/*
 * repeatly do "outsl"
 * write data from DS:ESI to port
 */
static inline void outsl(unsigned short port, void *pos, unsigned int count)
{
	__asm__ __volatile__ (
		"cld\n\t"
		"rep outsl"
		:"=&S"(pos), "=&c"(count)
		:"0"(pos), "d"(port), "1"(count)
		:"cc");
}

/*
 * inb %dx, %al
 */
static inline unsigned char inb(unsigned short port)
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
 * inw %dx, %ax
 */
static inline unsigned short inw(unsigned short port)
{
	unsigned short ret;
	__asm__ __volatile__ (
		"inw %1, %0"
		:"=a"(ret)
		:"d"(port):
		);
	return ret;
}

/*
 * inl %dx, %eax
 */
static inline unsigned int inl(unsigned short port)
{
	unsigned int ret;
	__asm__ __volatile__(
		"inl %1, %0"
		:"=a"(ret)
		:"d"(port):
		);
	return ret;
}

/*
 * repeatly do "insl"
 * read data from port to ES:EDI
 */
static inline void insl(unsigned short port, void *pos, unsigned int count)
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
static inline void stosb(void *pos, unsigned char data, unsigned int count)
{
	__asm__ __volatile__ (
		"cld\n\t"
		"rep stosb"
		:"=D"(pos), "=c"(count)
		:"D"(pos), "c"(count), "a"(data)
		:"cc", "memory"
		);
}

/*
 * repeatly do "stosl"
 * save data in EAX to ES:EDI
 */
static inline void stosl(void *pos, unsigned int data, unsigned int count)
{
	__asm__ __volatile__ (
		"cld\n\t"
		"rep stosl"
		:"=D"(pos), "=c"(count)
		:"D"(pos), "c"(count), "a"(data)
		:"cc", "memory"
		);
}

/*
 * Block interrupts except NMI
 */

static inline void cli()
{
	__asm__ __volatile__ ("cli");
}

/*
 * Enable interrupts
 */
static inline void sti()
{
	__asm__ __volatile__ ("sti");
}
