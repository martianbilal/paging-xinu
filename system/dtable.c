#include <xinu.h>


void alloc_dtable_pd(pid32 pid){

	proctab[pid].pd = new_dtable_entry(pid, pd, 5);
}

void alloc_dtable_shared_pts(void){

	int i;
	for (i=1; i<6; ++i){
		insert_pt(new_dtable_entry(NULLPROC, pt, 1024), NULLPROC);
	}
}

dentry_t *new_dtable_entry(pid32 pid, type_t type, uint32 used_entries){

	dentry_t *d_entry = get_d_entry();
	d_entry->pid = pid;
	d_entry->type = type;
	d_entry->used_entries = used_entries;
	return d_entry;
}

dentry_t *get_d_entry(void){

	int i;
	for (i=0; i<NDFRAME; ++i){
		if (dtable[i].pid == -1){
			return &dtable[i];
		}
	}
	return NULL;
}

void print_dtable(void){

	int i;
	for (i=0; i<NDFRAME; i++)
			kprintf("dtable[%d]: (pid=%d) (type=%d) (used_entries=%d) (address=0x%x)\n", i, 
															dtable[i].pid, dtable[i].type, 
															dtable[i].used_entries, 
															dtable[i].address);
}

void print_proc_dinfo(pid32 pid){

	struct	procent	*prptr;
	prptr = &proctab[pid];

	kprintf("Page Directory: (address=0x%x) (used_entries=%d)\n", prptr->pd->address, prptr->pd->used_entries);
	print_pts(pid);
}