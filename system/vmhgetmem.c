#include <xinu.h>

char *vmhgetmem(uint16 msize){

	uint32 page_number, va;

	page_number = alloc_ptable_pages(currpid, msize);
	if (page_number == SYSERR) return SYSERR;
	va = get_va(page_number);
	if (test_alloc_pt(currpid, va) == SYSERR) return SYSERR;
	return (char *) va;
}