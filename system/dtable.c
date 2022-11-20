#include <xinu.h>

/* Allocate a dtable entry for a page directory and update the pre-process relevant bookkeping side */
status alloc_dtable_pd(pid32 pid){

	dentry_t *dentry = new_dtable_entry(pid, pd, 5);
	if (dentry == NULL) return SYSERR;
	proctab[pid].pd = dentry;
	return OK;
}

/* Allocate the dtable entries for the shared page tables and update the pre-process relevant bookkeping side */
void alloc_dtable_shared_pts(void){

	int i;
	for (i=1; i<6; ++i){
		insert_pt(new_dtable_entry(NULLPROC, pt, 1024), NULLPROC);
	}
}

/* Allocate a dtable entry for a page table and update the pre-process relevant bookkeping side */
status alloc_dtable_pt(pid32 pid){

	dentry_t *dentry = new_dtable_entry(pid, pt, 0);
	if (dentry == NULL) return SYSERR;
	insert_pt(dentry, pid);
	proctab[pid].pd->used_entries++;
	return OK;
}

/* Deallocate all the dtable entries of the given process */
void dealloc_dtable_entries(pid32 pid){

	int i;
	for (i = 0; i < NDFRAME; i++){
		if(dtable[i].pid == pid){
			dtable[i].pid = -1;
			dtable[i].used_entries = 0;
		}	
	}
}

/* Searches for an empty dentry in dtable, and if finds one, it sets it up */
dentry_t *new_dtable_entry(pid32 pid, type_t type, uint32 used_entries){

	dentry_t *dentry = get_dentry();
	if (dentry == NULL) return NULL;
	dentry->pid = pid;
	dentry->type = type;
	dentry->used_entries = used_entries;
	return dentry;
}

/* Returns a pointer to an empty entry in dtable or NULL if all are occupied */
dentry_t *get_dentry(void){

	int i;
	for (i=0; i<NDFRAME; ++i){
		if (dtable[i].pid == -1) return &dtable[i];
	}
	return NULL;
}

/* Prints the dtable */
void print_dtable(void){

	int i;
	for (i=0; i<NDFRAME; i++)
			kprintf("dtable[%d]: (pid=%d) (type=%s) (used_entries=%d) (address=0x%x)\n", i, 
															dtable[i].pid,
															(dtable[i].type == pd)? "pd" : "pt", 
															dtable[i].used_entries, 
															dtable[i].address);
}

/* Prints the per-process D region information */
void print_proc_dinfo(pid32 pid){

	struct	procent	*prptr;
	prptr = &proctab[pid];

	kprintf("pd: (address=0x%x) (used_entries=%d)\n", prptr->pd->address, prptr->pd->used_entries);
	print_pts(pid);
}