#include <xinu.h>

/* Searches for an empty entry in e1table, and finds one, it allocates it and updates the per-process ptable */
status alloc_e1table_entry(pid32 pid, uint32 page_number){

	eentry_t *e1entry = new_e1table_entry(pid, page_number);
	if (e1entry == NULL) return SYSERR;
	proctab[pid].ptable[page_number].loc = e1;
	proctab[pid].ptable[page_number].eentry = e1entry;
	return OK;
}

/* Searches for an empty entry in the e1table, and if finds one, it allocates it */
eentry_t *new_e1table_entry(pid32 pid, uint32 page_number){

	eentry_t *e1entry = get_e1entry();
	if (e1entry == NULL) return NULL;
	e1entry->pid = pid;
	e1entry->page_number = page_number;
	return e1entry;
}

/* Returns a pointer to an empty entry in the e1table or NULL if all are occupied */
eentry_t *get_e1entry(void){

	int i;
	for (i=0; i<NE1FRAME; ++i){
		if (e1table[i].pid == -1){
			return &e1table[i];
		}
	}
	return NULL;
}

/* Prints the e1table */
void print_e1table(void){

	int i;
	for (i=0; i<NE1FRAME; i++)
			kprintf("e1table[%d]: (pid=%d) (page_number=%d) (address=0x%x)\n", i, 
															e1table[i].pid, e1table[i].page_number, 
															e1table[i].address);
}

/* Prints the per-process virtual address space */
void print_proc_einfo(pid32 pid){

	struct	procent	*prptr;
	prptr = &proctab[pid];

	kprintf("Per-process Virtual Address Space:\n");
	int i;
	for (i = 0; i < NPROCPAGE; i++){
		kprintf("ptable[%d] (location=%d) (address=0x%x)\n", i, prptr->ptable[i].loc, (!prptr->ptable[i].loc) ? -1 : prptr->ptable[i].eentry->address);
	}
}