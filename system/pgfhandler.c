/* pgfhandler.c - pgfhandler */

#include <xinu.h>


// static uint32 cr3;
// static uint32 cr2;


/*-------------------------------------------------------------------------
 * pgfhandler - paging fault handler
 *-------------------------------------------------------------------------
 */
void pgfhandler()
{
	struct procent *prptr;
    intmask mask;
	uint32 virt_addr;
	//uint32 virt_page_tab, virt_page_dir;

	mask = disable();
	
	virt_addr = getcr2();

	// Check if the page is not assigned (loc == empty)
	if(page_not_assigned(currpid, virt_addr)){
		kprintf("Segmentation Fault!\n");
		kill(currpid);
	}

	// Check if the page access violates the access rights
	if (page_exists(currpid, virt_addr) && access_violation(virt_addr)){
		kprintf("Access Violation!\n");
		kill(currpid);
	}

	// Check if the page is assigned and allocated to swap space (loc == e2)
	if (page_swapped(currpid, virt_addr)){
		//kprintf("assigned swapped\n");

		// There is an available E1 frame
		if (fetch_page(currpid, virt_addr) != SYSERR) goto ret;

		// There is not an available E1 frame, so eviction is needed
		evict_and_fetch_page(currpid, virt_addr);

	}else{ // Check if the page is assigned but not allocated (loc == vmem) 
		//kprintf("assigned not allocated\n");
		
		// There is an available E1 frame
		if (assign_page(currpid, virt_addr) != SYSERR) goto ret;
		
		// There is not an available E1 frame, so eviction is needed
		if (e2table_has_space()){
			evict();
			assign_page(currpid, virt_addr); // Cannot be SYSERR since we freed an entry above
		}else{
			// There is no space in E2, so process should be sent to sleep queue
			//kprintf("wait for frame\n");
			prptr = &proctab[currpid];
			prptr->prstate = PR_FRAME;
			prptr->pgf_virt_addr = virt_addr;
			enqueue(currpid, framewait);
			resched();
		}
	}

ret:
	restore(mask);
	return;
	
}

/* Evicts a frame from E1 region and fetches in its place a frame from E2 region */
void evict_and_fetch_page(pid32 pid, uint32 virt_addr){

	uint32 page_number = get_pte_num(virt_addr);

	// Deallocate the e2entry of the page-faulted frame and store its address in pf_e2entry
	eentry_t *pf_e2entry = proctab[pid].ptable[page_number].eentry;
	dealloc_e2table_entry(pf_e2entry);

	// Get the E1 frame that is going to be evicted
	eentry_t *e1entry = evict_buf_get(); // Cannot be NULL since E1 should be full to be here

	// Allocate an e2entry to the evicted frame and store its address in empty_e2entry
	alloc_e2table_entry(e1entry->pid, e1entry->page_number); // Cannot be SYSERR since we freed an entry above
	eentry_t *empty_e2entry = proctab[e1entry->pid].ptable[e1entry->page_number].eentry;

	// Invalidate the pte of the evicted frame
	set_p_pte(e1entry->pid, get_va(e1entry->page_number), 0x0);

	// Deallocate the e1entry for the evicted frame
	dealloc_e1table_entry(e1entry);

	// Allocate the previously freed e1entry to the page-faulted frame
	alloc_e1table_entry(pid, page_number); //Cannot be SYSERR since we freed an entry above

	// Update the pte of the page-faulted frame
	set_p_pte(pid, virt_addr, e1entry->address + pt_lsb12);
	
	// Swap or copy the page contents
	if (empty_e2entry == pf_e2entry){
		// TMP<-E1, E1<-E2, E2<-TMP
		swap_pages(e1entry->address, empty_e2entry->address);
	}else{
		// em_E2<-E1, E1<-pf_E2 
		copy_page(empty_e2entry->address, e1entry->address);
		copy_page(e1entry->address, pf_e2entry->address);
	}
}

/* Fetches in E1 region a frame that was in E2 region */
status fetch_page(pid32 pid, uint32 virt_addr){
	
	uint32 page_number = get_pte_num(virt_addr);	// page number of the virtual address
	uint32 e2_address = proctab[pid].ptable[page_number].eentry->address;
	if (alloc_e1table_entry(pid, page_number) == SYSERR) return SYSERR;
	uint32 e1_address = proctab[pid].ptable[page_number].eentry->address;
	copy_page(e1_address, e2_address);
	set_p_pte(pid, virt_addr, e1_address + pt_lsb12);
	return OK;
}

/*
	assign_page - assign a physical page to the virtual address
	returns OK on success, SYSERR on failure
*/
status assign_page(pid32 pid, uint32 virt_addr){
	
	uint32 page_number = get_pte_num(virt_addr);	// page number of the virtual address
	if (alloc_e1table_entry(pid, page_number) == SYSERR) return SYSERR;
	uint32 phys_address = proctab[pid].ptable[page_number].eentry->address;
	zero_p_mem(phys_address);
	set_p_pte(pid, virt_addr, phys_address + pt_lsb12);
	return OK;
}

/* Evicts a frame from the E1 region to the E2 region */
status evict(){

	// Get the E1 frame that is going to be evicted
	eentry_t *e1entry = evict_buf_get();
	if (e1entry == NULL) return SYSERR;

	// Allocate an e2entry to the evicted frame and store its address in empty_e2entry
	if (alloc_e2table_entry(e1entry->pid, e1entry->page_number) == SYSERR) return SYSERR;
	eentry_t *empty_e2entry = proctab[e1entry->pid].ptable[e1entry->page_number].eentry;

	// Invalidate the pte of the evicted frame
	set_p_pte(e1entry->pid, get_va(e1entry->page_number), 0x0);

	// Deallocate the e1entry for the evicted frame
	dealloc_e1table_entry(e1entry);

	// Copy the contents of the evicted frame to the corresponding E2 frame
	copy_page(empty_e2entry->address, e1entry->address);

	return OK;
}


/* Checks whether the given page is assigned or not by the process */
int page_not_assigned(pid32 pid, uint32 virt_addr){
	
	uint32 page_number = get_pte_num(virt_addr);	// page number of the virtual address
	loc_t loc = proctab[pid].ptable[page_number].loc;
	return (loc == empty);
}

/* Checks whether the given page resides in the E2 region */
int page_swapped(pid32 pid, uint32 virt_addr){
	
	uint32 page_number = get_pte_num(virt_addr);	// page number of the virtual address
	loc_t loc = proctab[pid].ptable[page_number].loc;
	return (loc == e2);
}

/*
	access_pf - returns 1 if the page fault was caused by access violation
*/
int access_violation(uint32 virt_addr){

	int writable = is_page_writable(currpid, virt_addr);
	uint32 access = pferrorcode & 0x2;
	if(writable == 0 && access == 2)
		return 1;
	else
		return 0;

}

/* Debugging funcs */

uint32 getcr2(){
	uint32 cr2 = 0;
	asm("movl %%cr2, %0" : "=r"(cr2));
	return cr2;
}

uint32 getcr3(){
	uint32 cr3 = 0;
	asm("movl %%cr3, %0" : "=r"(cr3));
	return cr3;
}

void printesp(pid32 pid){
	uint32 esp = 0;
	asm("movl %%esp, %0" : "=r"(esp));
	if (esp<proctab[currpid].prstkbase-proctab[currpid].prstklen)
		kprintf("PID: %d, SP: 0x%x\n", pid, esp);
}