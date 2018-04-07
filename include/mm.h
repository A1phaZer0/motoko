/*
 * Memory Management
 */

#define NULL		0x0
#define EXT_MEM 	0x100000	// Extended memory start from 1MB
#define KERN_BASE	0x80000000	// Kernel base address
#define KERN_ENTRY	0x80100000	// Kernel entry point
#define PG_SIZE 	4096		// PAGE SIZE in bytes
#define PDE_NUM		1024		// Entries number of page directory
#define PTE_NUM		1024		// Entries number of page directory
#define KSTACKSIZE	4096		// Kernel stack size
#define PHYEND		0xE0000000	// Physical address end

/*
 * Kernel Memory Space
 */
#define K_CODE (0x0008)	// IDX=1, TI=0(GDT), RPL=0
#define K_DATA (0x0010)	// IDX=2

/*
 * Identity Mapping before paging enabled.
 */
#define P2L(addr) ((addr) + 0x80000000) // Physical to Linear
#define L2P(addr) ((addr) - 0x80000000) // Linear to Physical
#define L2P_CAST(addr) ((unsigned int)(addr) - 0x80000000) // Linear to Physical

/*
 * Page Table Entry Type
 */
#define PTE_P	0x00000001	// Present
#define PTE_W	0x00000002	// Writable
#define PTE_U	0x00000004	// User
#define PTE_A	0x00000020	// Accessed
#define PTE_D	0x00000040	// Dirty

#define PDINX(la) ((la) >> 22 & 0x3FF)	// Page directory index
#define PTINX(la) ((la) >> 12 & 0x3FF)	// Page table index
#define PGOFFSET(la) ((la) & 0xFFF)	// Offset in a page

#define PGADDR(pg_entry)  ((pg_entry) & 0xFFFFF000)	// Page base address 
#define PGFLAG(pg_entry)  ((pg_entry) & 0x00000FFF)	// Page flags
#define PGROUNDUP(addr)	  (((unsigned)(addr)+ PG_SIZE - 1) & 0xFFFFF000)
#define PGROUNDDOWN(addr) ((unsigned)(addr) & 0xFFFFF000) // Page round down

/*
 * To avoid "no such instruction" compilation error when include this header
 *+into a asm source code 
 */
#ifndef __ASSEMBLER__

typedef unsigned int pte_t;
typedef unsigned int pde_t;

extern pte_t kern_pgtbl[];

void memset(void *, unsigned char, unsigned int);

struct list {
	struct list *next;
};

void setup_freelist(char *lstart, char *lend);
char *page_alloc(void);
void init_core_mem_map();

#endif
