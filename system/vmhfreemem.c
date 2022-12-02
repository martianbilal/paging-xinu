#include <xinu.h>

syscall vmhfreemem(char *blockaddr, uint16 msize){
	
	intmask mask = disable();

	if (msize < 1) goto ret_error;
	if (!is_valid_va((uint32)blockaddr)) goto ret_error;
	if (!pt_exists(currpid, (uint32)blockaddr)) goto ret_error;

	uint32 start_page = get_pte_num((uint32)blockaddr);
	uint32 end_page = start_page + msize;
	if (end_page > 1024) goto ret_error;

	uint32 cur_page;
	loc_t loc;
	eentry_t *eentry;
	
	for(cur_page = start_page; cur_page < end_page; cur_page++){
		loc = proctab[currpid].ptable[cur_page].loc;
		eentry = proctab[currpid].ptable[cur_page].eentry;
		
		if (loc == e1){
			dealloc_e1table_entry(eentry);
			set_p_pte(currpid, get_va(cur_page), 0x0);
			proctab[currpid].ptable[cur_page].loc = empty;
			proctab[currpid].ptable[cur_page].eentry = NULL;		
		}
		else if(loc == e2){
			dealloc_e2table_entry(eentry);
			evict();
			proctab[currpid].ptable[cur_page].loc = empty;
			proctab[currpid].ptable[cur_page].eentry = NULL;
		}
	}

	reset_evict_buf();
	assign_and_wakeup();
	goto ret_ok;


ret_error:
	restore(mask);
	return SYSERR;

ret_ok:
	restore(mask);
	return OK;

}


void assign_and_wakeup(void){

	//ready(dequeue(framewait));

	resched_cntl(DEFER_START);
	
	while (nonempty(framewait) && get_e1entry()!=NULL) {
		//kprintf("assign_and_wakeup IN\n");
		pid32 deq_pid = dequeue(framewait);
		assign_page(deq_pid, proctab[deq_pid].pgf_virt_addr);
		ready(deq_pid);
	}
	//kprintf("assign_and_wakeup while OUT\n");
	resched_cntl(DEFER_STOP);
	//kprintf("assign_and_wakeup OUT\n");


	return;
}