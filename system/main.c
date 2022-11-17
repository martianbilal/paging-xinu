/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

static uint32 cr3;
extern uint32 pd_n_addr;

process	main(void)
{

	/* Start the network */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	netstart();

	/* Initialize the page server */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	// psinit();

	// Insert test code below

	kprintf("Hello World\n");

// Testing the proc->pd changing
/*
	asm("movl %cr3, %eax");
	asm("movl %eax, cr3");
	kprintf("0x%x\n",cr3);0x40b003 0x409003
	0x8FFFF 000
	1000111111 1111111111
*/	

	//test_alloc_pt(currpid, 0x8FFFF000);
	//test_alloc_pt(currpid, 0x13FC000);
	//test_alloc_pt(currpid, 0x14FC000);
	//test_alloc_pt(currpid, 0x15FC000);
	//dealloc_pts_and_pd(currpid);
	//print_mem((uint32 *)pd_n_addr, (uint32 *)0x40b000);
	//print_mem_regions();
	//print_dtable();
	//print_proc_dinfo(currpid);


	return OK;
}
