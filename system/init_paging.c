#include <xinu.h>

extern void *_end;
extern void	*maxheap;

uint32 a_addr, b_addr, c_addr, d_addr, e1_addr, e2_addr, f_addr, g_addr;
uint32 a_frames, b_frames, c_frames, d_frames, e1_frames, e2_frames, f_frames, g_frames;
uint32 pd_n_addr;
uint32 pt_a_addr, pt_g_addr, pt_m_addr;

dentry_t dtable[NDFRAME];
eentry_t e1table[NE1FRAME];
eentry_t e2table[NE2FRAME];

void init_paging(void){

	
	init_globals();
	
	init_paging_structs();
	
	alloc_dtable_pd(NULLPROC);
	alloc_dtable_shared_pts();

	alloc_shared_pts();
	alloc_common_pd(NULLPROC);

	enable_paging();
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

	pt_a_addr = pd_n_addr + FRAME_SIZE;
	pt_g_addr = pd_n_addr + 5*FRAME_SIZE;
	pt_m_addr = pd_n_addr + 6*FRAME_SIZE;	
}

void init_paging_structs(void){

	int i;
	/* Initialize dtable */
	for (i = 0; i < NDFRAME; i++) {
		dtable[i].pid = -1;
		dtable[i].used_entries = 0;
		dtable[i].address = (d_addr + (i*FRAME_SIZE));
	}
	/* Initialize e1table */
	for (i = 0; i < NE1FRAME; i++) {
		e1table[i].pid = -1;
		e1table[i].page_number = -1;
		e1table[i].address = (e1_addr + (i*FRAME_SIZE));
	}
	/* Initialize e1table */
	for (i = 0; i < NE2FRAME; i++) {
		e2table[i].pid = -1;
		e2table[i].page_number = -1;
		e2table[i].address = (e2_addr + (i*FRAME_SIZE));
	}
}