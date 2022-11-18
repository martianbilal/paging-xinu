/* pgfhandler.c - pgfhandler */

#include <xinu.h>


// static uint32 cr3;
// static uint32 cr2;


uint32 find_free_e1();
uint32* find_pte_addr(pid32 pid, uint32 virt_addr);
uint32 clear_e1_page(uint32 e1_addr);

int assigned_physical(uint32 virt_addr);
int pde_present(pid32 pid, uint32 virt_addr);
int assign_page(pid32 pid, uint32 virt_addr);

int getcr2(void);
int getcr3(void);

/*-------------------------------------------------------------------------
 * pgfhandler - paging fault handler
 *-------------------------------------------------------------------------
 */
syscall pgfhandler()
{
	struct procent *prptr;
    intmask mask = disable();
	uint32 virt_addr = getcr2();
	// int pf_not_physical = 0;

	uint32 virt_page_dir = virt_addr >> 22;
	uint32 virt_page_tab = (virt_addr >> 12) & 0x3FF;


	prptr = &proctab[currpid];

    kprintf("======= Paging fault handler called ==========\n");
	kprintf("Virtual address: 0x%x\n", virt_addr);
	kprintf("Virtual directory: 0x%x\n", virt_page_dir);
	kprintf("Virtual page table: 0x%x\n", virt_page_tab);

	// TODO : check if the page is not assigned a physical page

	if(!pde_present(currpid, virt_addr)){
		kprintf("pid %d does not have address\n", currpid, virt_addr);
		goto ret;
	}
	if(!assigned_physical(virt_addr)){
		// kprintf("Page not assigned a physical page\n");
		// TODO : for part 2, check if E1 and E2 both are full and then queue the process
		// if(e1_full()){
		// 	// nothing to do right now
		// }
		// TODO : check if only E1 is full, call swap
		// TODO : assign the physical page to the faulting address
		if (assign_page(currpid, virt_addr)){
			kprintf("Page assigned\n");
		}
		else{
			kprintf("Page not assigned\n");
			goto ret;
		}
		// TODO : check if the page has moved to the backing store
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
	return OK;
	
}

/*
	assign_page - assign a physical page to the virtual address
	returns 1 on success, 0 on failure
*/
int assign_page(pid32 pid, uint32 virt_addr){
	uint32 e1_addr = find_free_e1();
	uint32 *pte = find_pte_addr(pid, virt_addr);


	clear_e1_page(e1_addr);

	*pte = e1_addr + pd_lsb12;
	
	return 0;
}


uint32 find_free_e1(){
	
	return -1;
}

uint32* find_pte_addr(pid32 pid, uint32 virt_addr){
	
	return (uint32*)(-1);
}

uint32 clear_e1_page(uint32 e1_addr){
	
	return -1;
}

/*
	pde_present - checks if the page table is present for this address
	returns 1 yes, 0 otherwise 
*/
int pde_present(pid32 pid, uint32 virt_addr){
	return 0;
}




/*
	assigned_physical - check if the page is assigned a physical page
	returns 0 if not assigned a physical page, 1 otherwise
*/
int assigned_physical(uint32 virt_addr){
	return 0;
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