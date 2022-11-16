#include <xinu.h>

#define pd_lsb12 	((uint32) 0x3)
#define pt_lsb12 	((uint32) 0x3)

#define FRAME_SIZE 	4096


extern void *_end;
extern void	*maxheap;

uint32 a_addr, b_addr, c_addr, d_addr, e1_addr, e2_addr, f_addr, g_addr;
uint32 a_frames, b_frames, c_frames, d_frames, e1_frames, e2_frames, f_frames, g_frames;
uint32 pd_n_addr, pd_g_addr;
uint32 pt_a_addr, pt_g_addr, pt_m_addr;

d_info_t dtable[NDFRAME];


void init_paging(void){

	
	init_globals();
	
	init_paging_structs();
	
	alloc_dtable_pd(currpid);
	alloc_dtable_shared_pts();

	alloc_shared_pts();
	alloc_common_pd(currpid);

	enable_paging();

	/* Inspect */
	//print_mem((uint32 *)pd_n_addr, (uint32 *)pt_m_addr);
	//print_mem_regions();
	//print_dtable();
	//print_proc_dinfo();
}

void init_globals(void){

	a_addr = 0x000000;
	b_addr = 0x101000;
	c_addr = (uint32) &_end;
	d_addr = (uint32) maxheap;
	e1_addr = (d_addr + NDFRAME*FRAME_SIZE);
	e2_addr = (e1_addr + NE1FRAME*FRAME_SIZE);
	f_addr = (e2_addr + NE2FRAME*FRAME_SIZE);
	g_addr = 0x90000000;

	a_frames = ((b_addr - a_addr + 1)/FRAME_SIZE);
	b_frames = ((c_addr - b_addr + 1)/FRAME_SIZE);
	c_frames = ((d_addr - c_addr + 1)/FRAME_SIZE);
	d_frames = ((e1_addr - d_addr + 1)/FRAME_SIZE);
	e1_frames = ((e2_addr - e1_addr + 1)/FRAME_SIZE);
	e2_frames = ((f_addr - e2_addr + 1)/FRAME_SIZE);
	f_frames = ((g_addr - f_addr + 1)/FRAME_SIZE);
	g_frames = ((MEMLIMIT - g_addr + 1)/FRAME_SIZE);
	
	pd_n_addr = d_addr;
	pd_g_addr = (pd_n_addr + 576*4);

	pt_a_addr = pd_n_addr + FRAME_SIZE;
	pt_g_addr = pd_n_addr + 5*FRAME_SIZE;
	pt_m_addr = pd_n_addr + 6*FRAME_SIZE;	
}

void init_paging_structs(void){

	/* Initialize d_table */
	int i;
	for (i = 0; i < NDFRAME; i++) {
		dtable[i].pid = -1;
		dtable[i].used_entries = 0;
		dtable[i].address = (d_addr + (i*FRAME_SIZE));
	}
}

void alloc_dtable_pd(pid32 pid){

	proctab[pid].pd = new_dtable_entry(pid, pd, 5);
}

void alloc_dtable_shared_pts(void){

	int i;
	for (i=1; i<6; ++i){
		insert_pt(new_dtable_entry(NULLPROC, pt, 1024), NULLPROC);
	}
}

/* add dtable? pd pts new proc
void update_dtable(pid32 pid){

	int i;
	for (i=1; i<6; ++i){
		insert_pt(new_dtable_entry(pid, pt, 1024), pid);
	}
}
*/

d_info_t *new_dtable_entry(pid32 pid, type_t type, uint32 used_entries){

	d_info_t *d_entry = get_d_entry();
	d_entry->pid = pid;
	d_entry->type = type;
	d_entry->used_entries = used_entries;
	return d_entry;
}

d_info_t *get_d_entry(void){

	int i;
	for (i=0; i<NDFRAME; ++i){
		if (dtable[i].pid == -1){
			return &dtable[i];
		}
	}
	return NULL;
}

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

void print_mem(uint32 *start_addr, uint32 *end_addr){

	uint32 *entry_addr;
	for (entry_addr=start_addr; entry_addr<end_addr; entry_addr++)
			kprintf("0x%x: 0x%x\n", entry_addr, *(entry_addr));
}

void print_mem_regions(void){

	kprintf("Region A: [0x%x - 0x%x], #%d frames\n", a_addr, b_addr-FRAME_SIZE, a_frames);
	kprintf("Region B: [0x%x - 0x%x], #%d frames\n", b_addr, c_addr-FRAME_SIZE, b_frames);
	kprintf("Region C: [0x%x - 0x%x], #%d frames\n", c_addr, d_addr-FRAME_SIZE, c_frames);
	kprintf("Region D: [0x%x - 0x%x], #%d frames\n", d_addr, e1_addr-FRAME_SIZE, d_frames);
	kprintf("Region E1: [0x%x - 0x%x], #%d frames\n", e1_addr, e2_addr-FRAME_SIZE, e1_frames);
	kprintf("Region E2: [0x%x - 0x%x], #%d frames\n", e2_addr, f_addr-FRAME_SIZE, e2_frames);
	kprintf("Region F: [0x%x - 0x%x], #%d frames\n", f_addr, g_addr-FRAME_SIZE, f_frames);
	kprintf("Region G: [0x%x - 0x%x], #%d frames\n", g_addr, MEMLIMIT-FRAME_SIZE, g_frames);
}

void print_dtable(void){

	int i;
	for (i=0; i<NDFRAME; i++)
			kprintf("dtable[%d]: (pid=%d) (type=%d) (used_entries=%d) (address=0x%x)\n", i, 
															dtable[i].pid, dtable[i].type, 
															dtable[i].used_entries, 
															dtable[i].address);
}

void print_proc_dinfo(void){

	struct	procent	*prptr;
	prptr = &proctab[currpid];

	kprintf("Page Directory: (address=0x%x) (used_entries=%d)\n", prptr->pd->address, prptr->pd->used_entries);
	print_pts(currpid);
}