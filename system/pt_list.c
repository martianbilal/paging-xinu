#include <xinu.h>

void print_pts(pid32 pid) {

	pt_info_t *ptptr;
	for (ptptr = proctab[pid].pts; ptptr != NULL; ptptr = ptptr->next)
		kprintf("(address=0x%x) (used_entries=%d)\n", ptptr->d_entry->address, ptptr->d_entry->used_entries);
}

void insert_pt(d_info_t *d_entry, pid32 pid) {
	
	pt_info_t *pt = (pt_info_t*) getmem(sizeof(pt_info_t));
	pt->d_entry = d_entry;
	pt->next = proctab[pid].pts;
	proctab[pid].pts = pt;
}

int isempty_pt(pid32 pid) {
	return (proctab[pid].pts == NULL);
}

pt_info_t *find_pt(d_info_t *d_entry, pid32 pid) {

	pt_info_t* pt = proctab[pid].pts;

	if (isempty_pt(pid)) return NULL;

	for (; pt->d_entry != d_entry; pt = pt->next) {
		if (pt->next == NULL) return NULL;
	}      
	return pt;
}

void delete_pt(d_info_t *d_entry, pid32 pid) {

	pt_info_t* cur_pt = proctab[pid].pts;
	pt_info_t* prev_pt = NULL;
	
	if (isempty_pt(pid)) return;

	for (; cur_pt->d_entry != d_entry; prev_pt=cur_pt, cur_pt=cur_pt->next){
		if (cur_pt->next == NULL) return;
	}

	if (cur_pt == proctab[pid].pts) {
		proctab[pid].pts = proctab[pid].pts->next;
	} else{
		prev_pt->next = cur_pt->next;
	}    
	freemem((char *)cur_pt, sizeof(pt_info_t));
}