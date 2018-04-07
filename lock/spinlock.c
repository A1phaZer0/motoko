/*
 * A quick implementation of spin lock
 */

#include "../include/spinlock.h"
#include "../include/mm.h"
#include "../include/utils.h"

/*
 * Initialize spin lock
 */
void init_spinlock(spinlock *lk, char *name)
{
	lk->in_use = 0;
	lk->name = name;
}

/*
 * Aquire spin lock
 */
void aquire_spinlock(spinlock *lk)
{
	int dummy;
	cli();
	while (lk->in_use) {
		;
	}
	__asm__ __volatile__ (
		"lock\n\t"
		"xchg %1, %0"
		:"=b"(dummy)
		:"m"(*(&(lk->in_use))), "0"(1)
		:"memory");
}

/*
 * Release spin lock
 */
void release_spinlock(spinlock *lk)
{
	int dummy;
	__asm__ __volatile__ (
		"lock\n\t"
		"xchg %1, %0"
		:"=b"(dummy)
		:"m"(*(&(lk->in_use))), "0"(0)
		:"memory");
	sti();
}
