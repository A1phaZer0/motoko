/*
 * Memory Management
 */
#include "../include/mm.h"
#include "../include/utils.h"

extern char bdata[]; // Linker defined symbol

/*
 * Once mapping 0x80000000 to 0x00000000
 * then 0x80100000 will be mapped to 0x00100000
 */
__attribute__((__aligned__(PG_SIZE)))
pte_t kern_pgdir[PDE_NUM];


__attribute__((__aligned__(PG_SIZE)))
pte_t kern_pgtbl[PTE_NUM];

pde_t *core_pgdir;

/*
 * Implement this function in entry.S
 */
void init_kern_pgtbl(pte_t kern_pgtbl[])
{
	int pg_idx;		// Page index
	unsigned int phy_addr;	// Physical address for mapping

	phy_addr = 0;
	for (pg_idx = 0; pg_idx < PTE_NUM; pg_idx++) {
		kern_pgtbl[pg_idx] = phy_addr | PTE_W | PTE_P;
		phy_addr += PG_SIZE;	// next page
	}
}

/*
 * Set memory with data
 */
void memset(void *mem, unsigned char data, unsigned int count)
{
	stosb(mem, data, count);
}

/*
 * Core Memory Mapping
 * [0, EXT_MEM)    maps to [0x80000000, 0x80100000) for I/O space.
 * [EXT_MEM, data) maps to [0x80100000, data) for core image read only data.
 * [data, 2GB)     maps to [data, 0xFFFFFFFF) for writable data.
 */
static struct map{
	char *lnr_start;	// Linear address of start point
	unsigned int phy_start;	// Physical address of start point
	unsigned int phy_end;	// Physical ending
	unsigned int perm;	// permissions
} cmmap [] = {
	{(char *)KERN_BASE, 	0x0, 		EXT_MEM,	PTE_W},
	{(char *)KERN_ENTRY,	EXT_MEM,	L2P_CAST(bdata),  0x0},
	{(char *)bdata,		L2P_CAST(bdata),0x80000000,	PTE_W},
};

/*
 * Get page table entry corresponding to linear address la.
 * If page table itself doesn't exist, then allocate one.
 */
pte_t *get_page_table_entry(pde_t *pd, char *la)
{
	pte_t *page_table;

	if (pd[PDINX((int)la)] & PTE_P)
		page_table = (pte_t *)P2L(PGADDR(pd[PDINX((int)la)]));
	else {
		pd[PDINX((int)la)] = (pde_t)page_alloc();
		page_table = (pte_t *)pd[PDINX((int)la)];

		//Clear new allocated page table
		memset((void *)pd[PDINX((int)la)], 0, PG_SIZE);

		pd[PDINX((int)la)] |= PTE_P | PTE_W | PTE_U;
		pd[PDINX((int)la)] = (pde_t)L2P((int)pd[PDINX((int)la)]);
	}

	return &page_table[PTINX((int)la)];
}


/*
 * Map physical [pstart, pend) to linear [lstart, lend) with permission set
 */
void map_memory_space(pde_t *pd, char *lstart, unsigned int pstart, 
		unsigned int pend, unsigned int perm)
{
	pte_t *tbl_entry;
	lstart = (char *)PGROUNDDOWN(lstart);

	for (; pstart < pend; pstart += PG_SIZE) {
		tbl_entry = get_page_table_entry(pd, lstart);
	
		if (*tbl_entry & PTE_P) {
			//TODO : panic
			return;
		}
		*tbl_entry = pstart | perm | PTE_P;
		lstart += PG_SIZE;
	}
}

/*
 * Load CR3
 */
void loadCR3(unsigned int val)
{
	__asm__ __volatile__ (
		"mov %0, %%cr3"
		:
		:"r"(val)
		:);
		
}

/*
 * Rebuild core page directory
 */
void init_core_mem_map()
{
	int i;

	core_pgdir = (pde_t *)page_alloc();
	*core_pgdir |= PTE_P | PTE_W;
	for (i = 0; i < 3; i++)
		map_memory_space(core_pgdir, cmmap[i].lnr_start,
				cmmap[i].phy_start, cmmap[i].phy_end,
				cmmap[i].perm);
	loadCR3((unsigned int)L2P(core_pgdir));
}
