#include <xinu.h>

/*  */
#define pd_lsb12 (0x3)
#define pt_lsb12 (0x3)


//uint32 cr0;
//uint32 cr3;

void init_paging(void){

	/* mem[PDE_0] = 0x401000 (PT_ABC) */
	/*

	0. P = 1;
	1. R/W = 1;
	2. U = 0;
	3. PWT = 0;
	4. PCD = 0;
	5. A = 0;
	6. D = 0; Ignored
	7. PS = 0;
	8:11. 0; Ignored
	12:31. PT_addr/0x1000; 

0x401000: 10000000001000000000000

0x401000 >> 0xc = 0x401 frame
0x401000/0x1000	= 0x401 frame

0x402000: 10000000010000000000000

	*/

/* 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 */
/*  0  0  0  0  0  0  0  0  0  1  0  0  0  0  0  0  0  0  0  1  0  0 0 0 0 0 0 0 0 0 1 1 */



	/* mem[PDE_1] = 0x402000 (PT_DE1) */
// 0x402000 >> 0xc = 0x402 frame = 10000000010

/* 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 */
/*  0  0  0  0  0  0  0  0  0  1  0  0  0  0  0  0  0  0  1  0  0  0 0 0 0 0 0 0 0 0 1 1 */

/* 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 */
/*  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 0 0 0 0 0 0 0 0 1 1 */



	/* Initialize null's page directory */
	
	uint32 *pd_n_addr = (uint32 *) 0x400000;
	uint32 *pd_g_addr = (uint32 *) 0x400900;

	*(pd_n_addr) = (0x401000 + pd_lsb12); /* mem[PDE_0] = 0x401000 (PT_ABC) */
	*(pd_n_addr + 1) = (0x402000 + pd_lsb12); /* mem[PDE_1] = 0x402000 (PT_DE1) */
	*(pd_n_addr + 2) = (0x403000 + pd_lsb12); /* mem[PDE_2] = 0x403000 (PT_E1E2) */
	*(pd_n_addr + 3) = (0x404000 + pd_lsb12); /* mem[PDE_3] = 0x404000 (PT_E2) */
	*(pd_g_addr) = (0x405000 + pd_lsb12); /* mem[PDE_4] = 0x405000 (PT_G) */




	/* Initialize global page tables A, B, C, D, E1, E2, G */

	uint32 *pt_a_addr = (uint32 *) 0x401000;
	uint32 *pt_g_addr = (uint32 *) 0x405000;
	uint32 *pt_m_addr = (uint32 *) 0x406000;
	
	uint32 *entry_addr;
	uint32 entry_content = 0x0;
	
	for (entry_addr=pt_a_addr; entry_addr<pt_g_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}

	entry_content = 0x90000000;
	for (entry_addr=pt_g_addr; entry_addr<pt_m_addr; entry_addr++, entry_content += 0x1000){
		*(entry_addr) = (entry_content + pt_lsb12);
	}

	asm("movl $0x400000, %eax");
	asm("movl %eax, %cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");


/*
	for (entry_addr=pd_n_addr; entry_addr<pt_m_addr; entry_addr++)
		kprintf("%x: %x\n", entry_addr, *(entry_addr));
*/

/*	
	asm("movl $0x400000, %eax");
	asm("movl %eax, %cr3");
	//asm("movl %eax, cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	//asm("movl %eax, cr0");
	asm("movl %eax, %cr0");
	//kprintf("%x\n", cr3);
	//kprintf("%x\n", cr0);
*/

/*
	mov eax, page_directory
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
*/
}
