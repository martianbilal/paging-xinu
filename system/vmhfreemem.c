#include <xinu.h>

syscall vmhfreemem(char *blockaddr, uint16 msize){
	
	if (msize < 1) return SYSERR;
	if (!is_valid_va((uint32)blockaddr)) return SYSERR;
	if (!pt_exists(currpid, (uint32)blockaddr)) return SYSERR;

	uint32 start_page = get_pte_num((uint32)blockaddr);
	uint32 end_page = start_page + msize;
	if (end_page > 1024) return SYSERR;

	uint32 cur_page;
	loc_t loc;
	eentry_t *eentry;
	for(cur_page = start_page; cur_page < end_page; cur_page++){
		loc = proctab[currpid].ptable[cur_page].loc;
		if (loc == e1){
			eentry = proctab[currpid].ptable[cur_page].eentry;
			dealloc_e1table_entry(eentry);
			set_p_pte(currpid, get_va(cur_page), 0x0);
			proctab[currpid].ptable[cur_page].eentry = NULL;			
		}
		proctab[currpid].ptable[cur_page].loc = empty;
	}
	return OK;
}