/*
 * ELF data structure
 */

/*
 * ELF basic types
 */
typedef unsigned int 	Elf32_Addr;	// Alignment 4 size 4
typedef unsigned short	Elf32_Half;	// Alignment 2 size 2
typedef unsigned int 	Elf32_Off;
typedef int		Elf32_Sword;
typedef	unsigned int 	Elf32_Word;

/*
 * Magic Number
 */
#define EI_NIDENT	16

/*
 * ELF Header
 */
typedef struct {
	unsigned char	e_ident[EI_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word 	e_version;
	Elf32_Addr	e_entry;	// VMA of entry point
	Elf32_Off	e_phoff;	// Program header table offset
	Elf32_Off 	e_shoff;	// Section header table offset
	Elf32_Word 	e_flags;
	Elf32_Half	e_ehsize;	// ELF header size
	Elf32_Half 	e_phentsize;	// Size of table entry
	Elf32_Half 	e_phnum;	// Number of table entries
	Elf32_Half 	e_shentsize;
	Elf32_Half 	e_shnum;
	Elf32_Half 	e_shstrndx;	// Index of section associated
					//+string table in section header
					//+table
} Elf32_Ehdr;


/*
 * ELF Program Header
 * ELF Program Header Table Entry
 */
typedef struct {
	Elf32_Word	p_type;
	Elf32_Off	p_offset;	// File offset of segment
	Elf32_Addr	p_vaddr;	// VMA of first byte in segment
	Elf32_Addr	p_paddr;	// Physical address or LMA
	Elf32_Word	p_filesz;	// Number of bytes in the file
	Elf32_Word	p_memsz;	// Number of bytes in the memory space
	Elf32_Word	p_flags;	// 
	Elf32_Word	p_align;	// Alignment constraints
} Elf32_Phdr;



/*
 * ELF Section Header
 * ELF Section Header Table Entry
 */
typedef struct {
	Elf32_Word	sh_name;	// Index into section string table
	Elf32_Word	sh_type;	// Section type
	Elf32_Word	sh_flags;	// Section attributes
	Elf32_Addr	sh_addr;	// VMA of first byte in section
	Elf32_Off	sh_offset;	// File offset of section
	Elf32_Word	sh_size;	// Section's size
	Elf32_Word	sh_link;	//
	Elf32_Word	sh_info;	// Extra information
	Elf32_Word	sh_addralign;	// Alignment constraints
	Elf32_Word	sh_entsize;	// Entry size for table of 
					//+fixed-size entries
} Elf32_Shdr;

/*
 * e_type values
 */
enum { 
	ET_NONE	= 0,
	ET_REL	= 1,
	ET_EXEC	= 2,
	ET_DYN	= 3
};

/*
 * e_machine values
 */
enum {
	EM_NONE = 0,
	EM_386	= 3	// Intel 80386
};

/*
 * e_version values
 */
enum {
	EV_NONE 	= 0,
	EV_CURRENT	= 1
};

/*
 * p_type values
 */
enum {
	PT_NULL		= 0,
	PT_LOAD		= 1,
	PT_DYNAMIC	= 2,
	PT_INTERP	= 3,
	PT_NOTE		= 4,
	PT_SHLIB	= 5,
	PT_PHDR		= 6,
	PT_LOPROC	= 0x70000000,
	PT_HIPROC	= 0x7fffffff
};
