#include <xinu.h>

/* Return the number of the first page of the contiguous virtual address chunk or -1 */
uint32 alloc_ptable_pages(pid32 pid, uint16 num){

	struct	procent	*prptr;
	int i, j, counter, page_number;


	// kprintf("MSIZE : %d alloc_ptable_pages: pid=%d num=%d\n", MAXHSIZE, pid, num);

	if (num < 1 || num > MAXHSIZE) return SYSERR;

	prptr = &proctab[pid];
	counter = 0;
	page_number = 0;

	for (i = 0; i < MAXHSIZE; i++){
		if (prptr->ptable[i].loc == empty){
			counter++;
			if (counter == num){
				for (j = page_number; j <= i; j++){
					prptr->ptable[j].loc = vmem;
				}
				return page_number;
			}
		}else{
			counter = 0;
			page_number = i + 1;
		}
	}
	return SYSERR;
}