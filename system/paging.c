#include <xinu.h>

/* Allocate the shared page tables (PT_ABC, PT_DE1, PT_E1E2, PT_E2, PT_G) in the physical memory */
void alloc_shared_pts(void){

	uint32 *entry_addr;
	uint32 entry_content = 0x0;
	
	/* Initialize the shared page tables PT_ABC, PT_DE1, PT_E1E2, PT_E2 */
	for (entry_addr=(uint32 *)pt_a_addr; entry_addr<(uint32 *)pt_g_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}

	/* Initialize the shared page table G */
	entry_content = 0x90000000;
	for (entry_addr=(uint32 *)pt_g_addr; entry_addr<(uint32 *)pt_m_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}
}

/*
	p1=0x1000000
	p2=0x1001000
	p3=0x1002000
		...
	p1024=0x13FF000
			0x13FF (000): 12bits
			  |--> 13FF = pd=100(4) pt=1111111111(1023): 10 bits
*/
/* Allocate a page table corresponding to the given page and update dtable only if there isn't already one */
void test_alloc_pt(pid32 pid, uint32 p_addr){
	
	if (pt_exists(pid, p_addr)) return;
	alloc_dtable_pt(pid); // Allocate the a new entry in the dtable for the new page table
	uint32 pt_addr = proctab[pid].pts->dentry->address;
	zero_pt_mem(pt_addr);
	*(get_pt_pde(pid, p_addr)) = (pt_addr + pd_lsb12);
}

/* Check if there is already a pde for the page table corresponding to the given page */
int pt_exists(pid32 pid, uint32 p_addr){

	return (*(get_pt_pde(pid, p_addr))&0x1);
}

/* Takes a page and returns the address of the pde that corresponds to that page's page table */
uint32 *get_pt_pde(pid32 pid, uint32 p_addr){

	uint32 pd_addr = proctab[pid].pd->address;
	return	((uint32 *)(pd_addr + ((p_addr>>22)*4)));
}

/* Allocate the page directory and fill the first 5 pdes related to the shared page tables */
void alloc_common_pd(pid32 pid){

	uint32 pd_addr = proctab[pid].pd->address;
	zero_pd_mem(pd_addr);
	
	*((uint32 *)pd_addr) = (pt_a_addr + pd_lsb12); 						/* mem[PDE_0] = 0x401000 (PT_ABC) */
	*((uint32 *)pd_addr + 1) = ((pt_a_addr + FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_1] = 0x402000 (PT_DE1) */
	*((uint32 *)pd_addr + 2) = ((pt_a_addr + 2*FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_2] = 0x403000 (PT_E1E2) */
	*((uint32 *)pd_addr + 3) = ((pt_a_addr + 3*FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_3] = 0x404000 (PT_E2) */
	*((uint32 *)pd_addr + 576) = ((pt_a_addr + 4*FRAME_SIZE) + pd_lsb12); /* mem[PDE_4] = 0x405000 (PT_G) */
}

/* Set to zero all the memory allocated for the given pd */
void zero_pd_mem(uint32 pd_addr){

	uint32 pd_max_addr = pd_addr + FRAME_SIZE;
	for (; pd_addr<pd_max_addr; pd_addr+=4)
		*((uint32 *)pd_addr) = 0x0;
}

/* Set to zero all the memory allocated for the given pt */
void zero_pt_mem(uint32 pt_addr){

	uint32 pt_max_addr = pt_addr + FRAME_SIZE;
	for (; pt_addr<pt_max_addr; pt_addr+=4)
		*((uint32 *)pt_addr) = 0x0;
}

/* Deallocating the private page tables and directory of a process and update the dtable */
/* To be called on process exit */
void dealloc_pts_and_pd(pid32 pid){

	dealloc_dtable_pts(pid);
	delete_pts(pid);
	dealloc_dtable_pd(pid);
}

/* Enable paging */
void enable_paging(void){

	/* Enable paging */
	asm("movl pd_n_addr, %eax");
	asm("movl %eax, %cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}