/*
 * Motoko
 */
#include "include/mm.h"

extern char end[];

int main(void)
{
	setup_freelist(end, (char *)P2L(0x400000));
	init_core_mem_map();
	return 0;
}
