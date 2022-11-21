/* pgfhandler.c - pgfhandler */

#include <xinu.h>


// static uint32 cr3;
// static uint32 cr2;


uint32 find_free_e1();
uint32 get_e1_size();
uint32 find_pte_addr(pid32 pid, uint32 virt_addr);
status clear_e1_page(uint32 e1_addr);



int access_violation(uint32 virt_addr);
int not_allocated(uint32 virt_addr);
int assigned_physical(uint32 virt_addr);
int pde_present(pid32 pid, uint32 virt_addr);
int assign_page(pid32 pid, uint32 virt_addr);

int getcr2(void);
int getcr3(void);

/*-------------------------------------------------------------------------
 * pgfhandler - paging fault handler
 *-------------------------------------------------------------------------
 */
void pgfhandler()
{
	struct procent *prptr;
    intmask mask = disable();
	uint32 virt_addr = getcr2();
	// int pf_not_physical = 0;

	uint32 virt_page_tab = (virt_addr >> 12) & 0x3FF;
	uint32 virt_page_dir = virt_addr >> 22;

	prptr = &proctab[currpid];

    // kprintf("======= Paging fault handler called ==========\n");
	// kprintf("error code : 0x%x\n", pferrorcode);
	// // kprintf("EIP : 0x%x\n", pinstr);
	// // kprintf("eflags : 0x%x\n", peflags);
	// // kprintf("cs : 0x%x\n", pcs);

	// kprintf("Virtual address: 0x%x\n", virt_addr);
	// kprintf("Virtual directory number: 0x%x\n", virt_page_dir);
	// kprintf("Virtual page table number: 0x%x\n", virt_page_tab);

	// kprintf("%s, ", __func__);

	// TODO : check if the page is not assigned a physical page

	// check if the page was not allocated 
	if(not_allocated(virt_addr)){
		// kprintf("not allocated\n");
		kill(currpid);
		restore(mask);
		return;
	}


	// if(!pde_present(currpid, virt_addr)){
	// 	// kprintf("pid %d does not have address\n", currpid, virt_addr);
	// 	goto ret;
	// }
	if(!assigned_physical(virt_addr)){
		// // kprintf("Page not assigned a physical page\n");
		// TODO : for part 2, check if E1 and E2 both are full and then queue the process
		// if(e1_full()){
		// 	// nothing to do right now
		// }
		// TODO : check if only E1 is full, call swap
		// TODO : assign the physical page to the faulting address
		if (assign_page(currpid, virt_addr)){
			// kprintf("Page assigned\n");
		}
		else{
			// kprintf("Page not assigned\n");
			goto ret;
		}
		// TODO : check if the page has moved to the backing store
	} else {
		// kprintf("Page already assigned\n");
		if(access_violation(virt_addr)){
			// kprintf("Access violation\n");
			// kill(currpid);
			restore(mask);
			return;
		}
	}
	/*
		for(int i = 0; i < list.size; i++){
			if(not_physical[i] == virt_addr){
				not_physical = 1;
				break;
			}
		}
				not_physical = 1;
				break;
			}
		}
	
	*/


	// STATWORD ps;
	// disable(ps);
	// unsigned long faddr, pdbr, pt, dstaddr;
	// int vpno, ptfrm, store, pageth, freefrm, dst, i;
	// virt_addr_t faultaddr;	
	// unsigned int p,q;
	// pd_t *pde;
	// pt_t *pte;

	// faddr = read_cr2();	/* get the faulted address */
	// get_virt_addr(faddr, &faultaddr);
	// get_virt_page_num(faultaddr, &vpno);
	// pdbr = proctab[currpid].pdbr;

	// p = faultaddr.pd_offset;
	// q = faultaddr.pt_offset;
	
	// pt = pdbr + (sizeof(pd_t)) * p;		/* pt points to the pth page table; both pd_t and pt_t are 4 bytes wide */
 	// pde = (pd_t *) pt;
	
	// /* the pth page table is not present */
	// if(!pde->pd_pres){
	// 	allocate_pt(currpid, &ptfrm);
	// 	initialize_pt(ptfrm);
		
	// 	/* update page directory entry with details of newly allocated page table */
	// 	pde->pd_pres = 1;
	//         pde->pd_write = 1;
    //             pde->pd_user = 0;
    //             pde->pd_pwt = 0;
    //             pde->pd_pcd = 0;
    //             pde->pd_acc = 0;
    //             pde->pd_mbz = 0;
    //             pde->pd_fmb = 0;
    //             pde->pd_global = 0;
    //             pde->pd_avail = 0;
	// 	pde->pd_base = FRAME0 + ptfrm;
	// }

	// /* Paging in the faulted page */
	// bsm_lookup(currpid, faddr, &store, &pageth);

	// frm_tab[pde->pd_base - FRAME0].fr_refcnt++; 	/* one more entry in the page table */
	// get_frm(&freefrm);
	// dstaddr = (FRAME0 + freefrm) * NBPG;
	// read_bs((char *)dstaddr , store, pageth);

	// /* update inverted page table to reflect details of the page */
	// frm_tab[freefrm].fr_status = FRM_MAPPED;
	// frm_tab[freefrm].fr_pid = currpid;
	// frm_tab[freefrm].fr_type = FR_PAGE;
	// frm_tab[freefrm].fr_vpno = vpno;
	// frm_tab[freefrm].fr_wrtype = WR_BASIC;
	
	// /* for SC page replacement */
	// frm_tab[freefrm].fr_acc = 1;	
	// num_sc++;

	// /* first frame into the SC circular queue */
	// if(num_sc == 1)	
	// 	sc_curr = freefrm;

	// append_fifo(&fifohead, freefrm);	/* for FIFO page replacement	*/

	// /* update page table */
	// pte = (pt_t *) ((pde->pd_base * NBPG) + (sizeof(pt_t) * q));

	// pte->pt_pres = 1;	/* page is present */
	// pte->pt_base = FRAME0 + freefrm;

ret:
	restore(mask);
	return;
	
}

/*
	assign_page - assign a physical page to the virtual address
	returns 1 on success, 0 on failure
*/
status assign_page(pid32 pid, uint32 virt_addr){
	uint32 page_number = (virt_addr >> 12) & 0x3FF;	// page number of the virtual address

	// kprintf("%s[0x%x, 0x%x], ", __func__, virt_addr, page_number);
	


	// kprintf("allocating the page with page number %d\n", page_number);
	status ret = alloc_e1table_entry(pid, page_number);
	if(ret != OK)
	{
		// kprintf("Page not allocated\n");
		return SYSERR;
	}

	uint32 phys_address = proctab[pid].ptable[page_number].eentry->address;
	uint32 pte = find_pte_addr(pid, virt_addr);



	ret = clear_e1_page(phys_address);
	if(ret != OK)
	{
		// kprintf("Failed to clear E1 page\n");
		return SYSERR;
	}

	// kprintf("[0x%x, 0x%x]", pte, phys_address | 0x1);
	// *(uint32 *)pte = phys_address | 0x7;
	
	// *(uint32 *)pte = phys_address | 0x1;
	uint32 pte_content = proctab[currpid].ptable[page_number].eentry->address & 0xFFFFF000;
	set_p_pte(currpid, virt_addr, ( pte_content | 0x3));


	// uint32 page_number = get_pte((uint32)virt_addr);
	// alloc_e1table_entry(pid, page_number);
	// set_p_pte(pid, (uint32)virt_addr, (proctab[pid].ptable[page_number].eentry->address + pt_lsb12));
	// // *x = 'a';
	// // kprintf("%d\n", *x);



	return OK;
}


int not_allocated(uint32 virt_addr){
	uint32 page_number = (virt_addr >> 12) & 0x3FF;	// page number of the virtual address
	if(proctab[currpid].ptable[page_number].loc != vmem)
		return 1;
	return 0;
}

/*
	access_pf - returns 1 if the page fault was caused by access violation
*/
int access_violation(uint32 virt_addr){
	int writable = is_page_writeable(currpid, virt_addr);
	uint32 access = pferrorcode & 0x2;

	
	if(writable == 0 && access == 2)
		return 1;
	else
		return 0;

}

uint32 find_free_e1(){
	int i = 0;
	// goes through the E1 and finds the free E1 page
	for(i = 0; i < get_e1_size(); i++){
		// if()
	}
	return -1;
}

uint32 get_e1_size(){
	return 0;
}


uint32 find_pte_addr(pid32 pid, uint32 virt_addr){
	uint32 page_number = (virt_addr >> 12) & 0x3FF;	// page number of the virtual address
	uint32 pd_number = (virt_addr >> 22);	// page directory number of the virtual address
	uint32 pd = proctab[pid].pd->address;
	// kprintf("[%d, (pd)0x%x]\n", pid, pd);

	uint32 pd_addr = pd + (pd_number * 4);
	uint32 pt = *(uint32 *)pd_addr & 0xFFFFF000;
	// kprintf("pd[%d] = 0x%x\n", pd_number, pd + (pd_number * 4));
	// kprintf("pt = 0x%x\n", pt);


	uint32 pte = pt + (page_number * sizeof(uint32));

	return pte;

}

status clear_e1_page(uint32 e1_addr){
	int i = 0;
	for(i = 0; i < FRAME_SIZE; i++){
		*(uint32 *)(e1_addr + i) = 0;
	}
	return OK;
}

/*
	pde_present - checks if the page table is present for this address
	returns 1 yes, 0 otherwise 
*/
int pde_present(pid32 pid, uint32 virt_addr){
	
	// TODO : returning 1 for testing
	return 1;
}




/*
	assigned_physical - check if the page is assigned a physical page
	returns 0 if not assigned a physical page, 1 otherwise
*/
int assigned_physical(uint32 virt_addr){

	// TODO : need updates for handling E2 swaps
	return page_exists(currpid, virt_addr);	
}




int getcr2(){
	int cr2 = 0;
	asm("movl %%cr2, %0" : "=r"(cr2));
	// asm("movl %cr3, %eax");
	// asm("movl %eax, cr2");
	return cr2;
}

int getcr3(){
	int cr3 = 0;
	asm("movl %%cr3, %0" : "=r"(cr3));
	return cr3;
}