/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

static uint32 cr3;
extern uint32 pd_n_addr;
char *gva;

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
	//resume(create(sndA, 1024, 20, "sndA", 0, NULL));
	//print_mem((uint32 *)pd_n_addr, (uint32 *)0x40b000);
	//print_mem_regions();
	//print_dtable();
	//print_proc_dinfo(currpid);

	//alloc_e1table_entry(currpid, 0);
	//alloc_e1table_entry(currpid, 499);
	//kprintf("page: %d\n", alloc_ptable_pages(currpid, 524));
	//kprintf("page: %d\n", alloc_ptable_pages(currpid, 10));
	//print_e1table();
	//print_proc_einfo(currpid);

/*
	char *x = vmhgetmem(2);
	alloc_e1table_entry(currpid, 0);
	alloc_e1table_entry(currpid, 1);
	print_proc_einfo(currpid);
	char *y = vmhgetmem(2);
	alloc_e1table_entry(currpid, 2);
	alloc_e1table_entry(currpid, 3);
	print_proc_einfo(currpid);
	vmhfreemem(x, 3);
	//kprintf("%d\n",vmhfreemem(y, 2));
	print_proc_einfo(currpid);
*/
/*	
	kprintf("Before\n");
	print_e1table();
	resume(create(sndA, 1024, 20, "sndA", 0, NULL));
	sleep(10);
	kprintf("After\n");
	print_e1table();
*/
/*
	gva = vmhgetmem(1);
	uint32 page_number = get_pte((uint32)gva);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)gva, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	gva[0] = 'a';
    gva[1] = 'p';
    gva[2] = 'p';
    gva[3] = 'l';
    gva[4] = 'e';
    gva[5] = '\0';
	resume(create(sndA, 1024, 20, "sndA", 0, NULL));
	kprintf("%s\n", gva);
*/
	return OK;
}


void sndA(void){

	kprintf("SndA\n");

// Pseudo-page handler
/*
	char *va = vmhgetmem(1); //uint32 va = get_va_from_reg(); //not implemented
	uint32 page_number = get_pte((uint32)va);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	va[0]='a';
*/

/*	
	asm("movl %cr3, %eax");
	asm("movl %eax, cr3");
	kprintf("0x%x\n",cr3);
*/
/*	
	//print_dtable();
	print_e1table();
*/
/*
	gva = vmhgetmem(1);
	uint32 page_number = get_pte((uint32)gva);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)gva, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	gva[0] = 'o';
    gva[1] = 'r';
    gva[2] = 'a';
    gva[3] = 'n';
    gva[4] = 'g';
    gva[5] = 'e';
    gva[6] = '\0';
    kprintf("%s\n", gva);
*/
}