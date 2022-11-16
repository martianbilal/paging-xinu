#include <xinu.h>

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