#include <xinu.h>

#define pd_lsb12 (0x3)
#define pt_lsb12 (0x3)


uint32 *pd_n_addr = (uint32 *) 0x400000;
uint32 *pd_g_addr = (uint32 *) 0x400900;

uint32 *pt_a_addr = (uint32 *) 0x401000;
uint32 *pt_g_addr = (uint32 *) 0x405000;
uint32 *pt_m_addr = (uint32 *) 0x406000;

d_info_t dtable[NDFRAME];


void init_paging(void){

	init_paging_structs();

	init_nullproc_dtable();
	init_nullproc_pd();
	init_nullproc_pt();

	enable_paging();

	/* Inspect */
	print_mem(pd_n_addr, pt_m_addr);
	print_dtable();
	print_proc_dinfo(currpid);

}

void init_paging_structs(void){

	/* Initialize d_table */
	int i;
	for (i = 0; i < NDFRAME; i++) {
		dtable[i].pid = -1;
		dtable[i].used_entries = 0;
		dtable[i].address = (pd_n_addr + (i*1024)); //1024 instead of 4096 due to (uint32 *) steps
	}
}

void init_nullproc_dtable(void){

	/* null process page directory */
	dtable[0].pid = currpid;
	dtable[0].type = pd;
	dtable[0].used_entries = 5;

	proctab[currpid].pd = &dtable[0];

	/* null process (shared) page tables */
	int i;
	for (i=1; i<6; ++i){
		dtable[i].pid = currpid;
		dtable[i].type = pt;
		dtable[i].used_entries = 1024;

		proctab[currpid].pts[i-1] = &dtable[i];
	}
} 

void init_nullproc_pd(void){

	/* Initialize null's page directory */

	*(pd_n_addr) = (0x401000 + pd_lsb12); /* mem[PDE_0] = 0x401000 (PT_ABC) */
	*(pd_n_addr + 1) = (0x402000 + pd_lsb12); /* mem[PDE_1] = 0x402000 (PT_DE1) */
	*(pd_n_addr + 2) = (0x403000 + pd_lsb12); /* mem[PDE_2] = 0x403000 (PT_E1E2) */
	*(pd_n_addr + 3) = (0x404000 + pd_lsb12); /* mem[PDE_3] = 0x404000 (PT_E2) */
	*(pd_g_addr) = (0x405000 + pd_lsb12); /* mem[PDE_4] = 0x405000 (PT_G) */
}

void init_nullproc_pt(void){

	uint32 *entry_addr;
	uint32 entry_content = 0x0;
	
	/* Initialize null's (shared) page tables A, B, C, D, E1, E2, G */
	for (entry_addr=pt_a_addr; entry_addr<pt_g_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}

	entry_content = 0x90000000;
	for (entry_addr=pt_g_addr; entry_addr<pt_m_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}
}

void enable_paging(void){

	/* Enable paging */
	asm("movl $0x400000, %eax");
	asm("movl %eax, %cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}

void print_mem(uint32 *start_addr, uint32 *end_addr){

	uint32 *entry_addr;
	for (entry_addr=start_addr; entry_addr<end_addr; entry_addr++)
			kprintf("0x%x: 0x%x\n", entry_addr, *(entry_addr));
}

void print_dtable(void){

	int i;
	for (i=0; i<NDFRAME; i++)
			kprintf("dtable[%d]: (pid=%d) (type=%d) (used_entries=%d) (address=0x%x)\n", i, 
															dtable[i].pid, dtable[i].type, 
															dtable[i].used_entries, 
															dtable[i].address);
}

void print_proc_dinfo(pid32 pid){

	struct	procent	*prptr;
	prptr = &proctab[currpid];

	kprintf("Page Directory: (address=0x%x) (used_entries=%d)\n", prptr->pd->address, prptr->pd->used_entries);
	
	int i;
	for (i=0; i<NPTE; i++)
		if (prptr->pts[i] != NULL){
			kprintf("Page Table [%d]: (address=0x%x) (used_entries=%d)\n", i, prptr->pts[i]->address, prptr->pts[i]->used_entries);
		}
}