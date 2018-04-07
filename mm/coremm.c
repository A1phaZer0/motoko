#include "../include/spinlock.h"
#include "../include/utils.h"
#include "../include/mm.h"

extern char end[];	// First Linear address after kernel
void range_free(char *, char *);

struct {
	spinlock lock;
	struct list *freelist;
} memspace;

/*
 * Part of Linear/Physical addresses mapping has been crafted
 *+before core entry. 
 * [0x80000000, 0x80400000) <==> [0x00000000, 0x00400000)
 * Now puts unused pages to freelist for new page directory
 *+page table allocation.
 */
void setup_freelist(char *lstart, char *lend)
{
	lstart = (char *)PGROUNDUP(lstart);
	lend   = (char *)PGROUNDDOWN(lend);
	range_free(lstart, lend);
}

/*
 * Initialize core memory space data structure.
 */
void init_memspace(void)
{
	init_spinlock(&memspace.lock, "core_mem");
	memspace.freelist = NULL;
}

/*
 * Put a page into freelist
 */
void page_free(char *page)
{
	struct list *tmp;
	if((unsigned int)page%PG_SIZE || L2P(page) >= (char *)PHYEND)
		return;
	aquire_spinlock(&memspace.lock);

	// Insert page into head of free list
	tmp = (struct list *)page;
	tmp->next = memspace.freelist;
	memspace.freelist = tmp;

	release_spinlock(&memspace.lock);
}

/*
 * Free pages from linear address [lstart, lend)
 */
void range_free(char *lstart, char *lend)
{
	char *page = lstart;

	for (; page < lend; page += PG_SIZE)
		page_free(page);
}

/*
 * Allocate a page from freelist
 */
char *page_alloc(void)
{
	struct list *pg;

	aquire_spinlock(&memspace.lock);
	pg = memspace.freelist;
	if(pg)
		memspace.freelist = pg->next;
	release_spinlock(&memspace.lock);
	return (char *)pg;	// Returns NULL if no page available.
}
