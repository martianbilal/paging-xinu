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
	*((uint32 *)get_pt_pde(pid, p_addr)) = (pt_addr + pd_lsb12);
}

/* Check if there is already a pde for the page table corresponding to the given page */
int pt_exists(pid32 pid, uint32 p_addr){
			
	return (*((uint32 *)get_pt_pde(pid, p_addr))&0x1);
}

/* Takes a page and returns the address of the pde that corresponds to that page's page table */
uint32 get_pt_pde(pid32 pid, uint32 p_addr){

	uint32 pd_addr = proctab[pid].pd->address;
	return	(pd_addr + ((p_addr>>22)*4));
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

//100   1111111111 000000000000
//pde	pte		   page_offset
//4		1023	
//11111111111111111111000000000000 = 0xFFFFF000
// pde_main = curr.pd+(4*pde)
// *(pde_main) = pt_main+pt_lsb12
// pt_main = *(pde_main)&0xFFFFF000
// *(pt_main+4*pte) = 0x0

/* Set to zero the pte that corresponds to the given page */
void zero_pte(pid32 pid, uint32 p_addr){

	uint32 pt_base_addr = *((uint32 *)get_pt_pde(pid, p_addr));
	pt_base_addr = pt_base_addr&0xFFFFF000; //clean lsb12 flag
	//kprintf("ad: 0x%x\n", (pt_base_addr + get_pte(p_addr)*4));
	//kprintf("content: 0x%x\n", *((uint32 *)(pt_base_addr + get_pte(p_addr)*4)));
	*((uint32 *)(pt_base_addr + get_pte(p_addr)*4)) = 0x0;
	//kprintf("content: 0x%x\n", *((uint32 *)(pt_base_addr + get_pte(p_addr)*4)));
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


// 1000111111 (1111111111) 000000000000
// 1111111111110000000000
// 0000000000001111111111

int is_valid_va(char *blockaddr){

	return (is_valid_pt(blockaddr));
}

int is_valid_pt(char *blockaddr){

	uint32 pde = get_pde((uint32) blockaddr);
	return (pde == 4);
	//return ((pt_number>=4) && (pt_number<=575));
}

/*
// may be unnecessary
int is_valid_pte(char *blockaddr){

	uint32 pte = get_pte((uint32) blockaddr);
	return ((pte >= 0) && (pte <= 1023));
}
*/

uint32 get_pde(uint32 blockaddr){

	return (blockaddr>>0x16);
}

uint32 get_pte(uint32 blockaddr){

	return ((blockaddr>>0xC)&0x3FF);
}

uint32 get_va(uint32 page_number){

	return (f_addr + page_number*FRAME_SIZE);
}