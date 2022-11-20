#include <xinu.h>

void insert_pt(dentry_t *dentry, pid32 pid) {
	
	pt_info_t *pt = (pt_info_t*) getmem(sizeof(pt_info_t));
	pt->dentry = dentry;
	pt->next = proctab[pid].pts;
	proctab[pid].pts = pt;
}

int isempty_pt(pid32 pid) {
	return (proctab[pid].pts == NULL);
}

pt_info_t *find_pt(dentry_t *dentry, pid32 pid) {

	pt_info_t* pt = proctab[pid].pts;

	if (isempty_pt(pid)) return NULL;

	for (; pt->dentry != dentry; pt = pt->next) {
		if (pt->next == NULL) return NULL;
	}      
	return pt;
}

void delete_pt(dentry_t *dentry, pid32 pid) {

	pt_info_t* cur_pt = proctab[pid].pts;
	pt_info_t* prev_pt = NULL;
	
	if (isempty_pt(pid)) return;

	for (; cur_pt->dentry != dentry; prev_pt=cur_pt, cur_pt=cur_pt->next){
		if (cur_pt->next == NULL) return;
	}

	if (cur_pt == proctab[pid].pts) {
		proctab[pid].pts = proctab[pid].pts->next;
	} else{
		prev_pt->next = cur_pt->next;
	}    
	freemem((char *)cur_pt, sizeof(pt_info_t));
}

void delete_pts(pid32 pid){

	pt_info_t* cur_pt = proctab[pid].pts;
	pt_info_t* del_pt;

	for (; cur_pt != NULL; ){
		del_pt = cur_pt;
		cur_pt = cur_pt->next;
		freemem((char *)del_pt, sizeof(pt_info_t));
	}
	proctab[pid].pts = NULL;
}

void print_pts(pid32 pid) {

	pt_info_t *ptptr;
	for (ptptr = proctab[pid].pts; ptptr != NULL; ptptr = ptptr->next)
		kprintf("pt: (address=0x%x) (used_entries=%d)\n", ptptr->dentry->address, ptptr->dentry->used_entries);
}
