#include <xinu.h>

char tmp_page[FRAME_SIZE];

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

/* Allocate the page directory and fill the first 5 pdes related to the shared page tables */
void alloc_common_pd(pid32 pid){

	uint32 pd_addr = proctab[pid].pd->address;
	zero_pd_mem(pd_addr);
	
	*((uint32 *)pd_addr) = (pt_a_addr + pd_lsb12); 							/* mem[PDE_0] = 0x401000 (PT_ABC) */
	*((uint32 *)pd_addr + 1) = ((pt_a_addr + FRAME_SIZE) + pd_lsb12); 		/* mem[PDE_1] = 0x402000 (PT_DE1) */
	*((uint32 *)pd_addr + 2) = ((pt_a_addr + 2*FRAME_SIZE) + pd_lsb12);		/* mem[PDE_2] = 0x403000 (PT_E1E2) */
	*((uint32 *)pd_addr + 3) = ((pt_a_addr + 3*FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_3] = 0x404000 (PT_E2) */
	*((uint32 *)pd_addr + 576) = ((pt_a_addr + 4*FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_4] = 0x405000 (PT_G) */
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
status test_alloc_pt(pid32 pid, uint32 virt_addr){
	
	if (pt_exists(pid, virt_addr)) return OK;
	// Allocate a new entry in the dtable for the new pt
	if (alloc_dtable_pt(pid) == SYSERR) return SYSERR;
	uint32 pt_addr = proctab[pid].pts->dentry->address;
	zero_pt_mem(pt_addr); // Zero the ptes of the new pt
	set_pt_pde(pid, virt_addr, pt_addr); //Set the pde that corresponds to the new pt
	return OK;
}

/* Takes a page and returns the address of the pde that corresponds to that page's page table */
uint32 get_pt_pde_addr(pid32 pid, uint32 virt_addr){

	uint32 pd_addr = proctab[pid].pd->address;
	return	(pd_addr + (get_pde_num(virt_addr)*4));
}

/* Sets the pde entry that corresponds to the given page table */
void set_pt_pde(pid32 pid, uint32 virt_addr, uint32 pt_addr){

	*((uint32 *)get_pt_pde_addr(pid, virt_addr)) = (pt_addr + pd_lsb12);
}

/*
	100   1111111111 000000000000
	pde	  pte		 page_offset
	4	  1023		 (irrelevant)

	11111111111111111111000000000000 = 0xFFFFF000
	reset_lsb12_mask = 0xFFFFF000

	pde_main = curr.pd+(4*pde)
	*(pde_main) = pt_main+pt_lsb12
	pt_main = *(pde_main)&reset_lsb12_mask
	*(pt_main+4*pte) = content
*/

/* Takes a page and returns the address of the pte that corresponds to that page */
uint32 get_p_pte_addr(pid32 pid, uint32 virt_addr){

	/* Get the base address of page's page table */
	uint32 pt_base_addr = *((uint32 *)get_pt_pde_addr(pid, virt_addr));

	/* Clean the 12 less significant bits used as control flag */
	uint32 reset_lsb12_mask = 0xFFFFF000;
	pt_base_addr = (pt_base_addr&reset_lsb12_mask);

	/* Return the pte entry that corresponds to the given page */
	return (pt_base_addr + get_pte_num(virt_addr)*4);
}

/* Set to content the pte that corresponds to the given page */
void set_p_pte(pid32 pid, uint32 virt_addr, uint32 phys_addr){

	*((uint32 *) get_p_pte_addr(pid, virt_addr)) = (phys_addr);
}

/* Swap the contents of frames e1 and e2 */
void swap_pages(uint32 e1_addr, uint32 e2_addr){

	uint32 src, dst;

	uint32 e1_max_addr = e1_addr + FRAME_SIZE;
	uint32 e2_max_addr = e2_addr + FRAME_SIZE;

	// Temp = E1
	for (dst = 0, src = e1_addr; src < e1_max_addr; src++, dst++)
		tmp_page[dst] = *((char *)src);

	// E1 = E2
	for (dst = e1_addr, src = e2_addr; src < e2_max_addr; src++, dst++)
		*((char *)dst) = *((char *)src);

	// E2 = TMP
	for (dst = e2_addr, src = 0; src < FRAME_SIZE; src++, dst++)
		*((char *)dst) = tmp_page[src];
}

/* Copies the frame starting from src_addr, to the frame starting from dst_addr */
void copy_page(uint32 dst_addr, uint32 src_addr){
	
	uint32 src, dst;
	uint32 src_max_addr = src_addr + FRAME_SIZE;

	for (dst = dst_addr, src = src_addr; src < src_max_addr; src++, dst++)
		*((char *)dst) = *((char *)src);
}

/* Set to zero all the memory allocated for the given pd */
void zero_pd_mem(uint32 pd_addr){

	uint32 pd_max_addr = pd_addr + FRAME_SIZE;
	for (; pd_addr < pd_max_addr; pd_addr += 4)
		*((uint32 *)pd_addr) = 0x0;
}

/* Set to zero all the memory allocated for the given pt */
void zero_pt_mem(uint32 pt_addr){

	uint32 pt_max_addr = pt_addr + FRAME_SIZE;
	for (; pt_addr < pt_max_addr; pt_addr += 4)
		*((uint32 *)pt_addr) = 0x0;
}

/* Set to zero all the memory allocated for the given physical address */
void zero_p_mem(uint32 phys_addr){

	uint32 p_max_addr = phys_addr + FRAME_SIZE;
	for (; phys_addr < p_max_addr; phys_addr += 4)
		*((uint32 *)phys_addr) = 0x0;
}

/* Enable paging */
void enable_paging(void){

	/* Enable paging */
	asm("movl pd_n_addr, %eax");
	asm("movl %eax, %cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80010001, %eax");
	asm("movl %eax, %cr0");
}

/* Checks whether a page exists in the memory based on its pte */
int page_exists(pid32 pid, uint32 virt_addr){

	uint32 pte = *((uint32 *) get_p_pte_addr(pid, virt_addr));
	
	/* Check if the page exists */
	return (pte&0x1);
}

/* Checks whether a page is writable */
int is_page_writable(pid32 pid, uint32 virt_addr){

	uint32 pte = *((uint32 *) get_p_pte_addr(pid, virt_addr));

	/* Check if the page is writeable */
	return (pte&0x2);
}

/* Check if there is already a pde for the page table corresponding to the given page */
int pt_exists(pid32 pid, uint32 virt_addr){
			
	return (*((uint32 *)get_pt_pde_addr(pid, virt_addr))&0x1);
}

/* Check whether the given virtual address is valid based on the given details */
int is_valid_va(uint32 virt_addr){

	return (is_valid_pt(virt_addr));
}

/* A valid virtual addresses must belong in 4th pde's page table */
int is_valid_pt(uint32 virt_addr){

	uint32 pde = get_pde_num(virt_addr);
	return (pde == 4);
}

/* Returns the page directory entry number that points to the page table of the given page address */
uint32 get_pde_num(uint32 virt_addr){

	return (virt_addr>>0x16);
}

/* Returns the page table entry number that points to the given page address */
uint32 get_pte_num(uint32 virt_addr){

	return ((virt_addr>>0xC)&0x3FF);
}

/* Returns the virtual address that corresponds to the given page number of region F */
uint32 get_va(uint32 page_number){

	return (f_addr + page_number*FRAME_SIZE);
}