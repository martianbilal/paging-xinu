#include <xinu.h>

void alloc_shared_pts(void){

	uint32 *entry_addr;
	uint32 entry_content = 0x0;
	
	/* Initialize null's (shared) page tables A, B, C, D, E1, E2, G */
	for (entry_addr=(uint32 *)pt_a_addr; entry_addr<(uint32 *)pt_g_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}

	entry_content = 0x90000000;
	for (entry_addr=(uint32 *)pt_g_addr; entry_addr<(uint32 *)pt_m_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}
}

void alloc_common_pd(pid32 pid){

	uint32 *pd_addr = (uint32 *) proctab[pid].pd->address;
	*(pd_addr) = (pt_a_addr + pd_lsb12); 						/* mem[PDE_0] = 0x401000 (PT_ABC) */
	*(pd_addr + 1) = ((pt_a_addr + FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_1] = 0x402000 (PT_DE1) */
	*(pd_addr + 2) = ((pt_a_addr + 2*FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_2] = 0x403000 (PT_E1E2) */
	*(pd_addr + 3) = ((pt_a_addr + 3*FRAME_SIZE) + pd_lsb12); 	/* mem[PDE_3] = 0x404000 (PT_E2) */
	*(pd_addr + 576) = ((pt_a_addr + 4*FRAME_SIZE) + pd_lsb12); /* mem[PDE_4] = 0x405000 (PT_G) */
}

void enable_paging(void){

	/* Enable paging */
	asm("movl pd_n_addr, %eax");
	asm("movl %eax, %cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}