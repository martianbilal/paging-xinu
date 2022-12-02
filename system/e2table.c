#include <xinu.h>

/* Deallocates the given eentry */
void dealloc_e2table_entry(eentry_t *eentry){

	eentry->pid = -1;
	eentry->page_number = -1;
}

/* Searches for an empty entry in e2table, and finds one, it allocates it and updates the per-process ptable */
status alloc_e2table_entry(pid32 pid, uint32 page_number){
	eentry_t *e2entry = new_e2table_entry(pid, page_number);
	if (e2entry == NULL) return SYSERR;
	proctab[pid].ptable[page_number].loc = e2;
	proctab[pid].ptable[page_number].eentry = e2entry;
	return OK;
}

/* Searches for an empty entry in e2table, and if finds one, it sets it up */
eentry_t *new_e2table_entry(pid32 pid, uint32 page_number){

	eentry_t *e2entry = get_e2entry();
	if (e2entry == NULL) return NULL;
	e2entry->pid = pid;
	e2entry->page_number = page_number;
	return e2entry;
}

/* Returns a pointer to an empty entry in e1table or NULL if all are occupied */
eentry_t *get_e2entry(void){

	int i;
	for (i = 0; i < NE2FRAME; ++i){
		if (e2table[i].pid == -1) return &e2table[i];
	}
	return NULL;
}

/* Prints the e2table */
void print_e2table(void){

	int i;
	for (i = 0; i < NE2FRAME; i++)
			kprintf("e2table[%d]: (pid=%d) (page_number=%d) (address=0x%x)\n", i, 
															e2table[i].pid,
															e2table[i].page_number, 
															e2table[i].address);
}