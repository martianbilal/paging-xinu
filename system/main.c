/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

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

/*
	A. vmhfreemem tests: 
		1. msize < 1
		2. Invalid virtual address and borders
		3. Valid virtual address, but page table doesn't exist
		4. Requesting to free more than the available virtual space
*/

/* A.1. */
/*
	char *va = vmhgetmem(1);
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)va);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	va[0] = 'a';
	va[1] = 'p';
	va[2] = 'p';
	va[3] = 'l';
	va[4] = 'e';
	va[5] = '\0'; //important
	kprintf("[va]: 0x%x -> %s\n", va, va);
	// Should return SYSERR
	kprintf("Return value: %s\n", ((vmhfreemem(va, 0) == SYSERR) ? "SYSERR" : "OK"));
	// Should return SYSERR
	kprintf("Return value: %s\n", ((vmhfreemem(va, -1) == SYSERR) ? "SYSERR" : "OK"));
*/	

/* A.2. */
/*
	char *va = vmhgetmem(1);
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)va);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	va[0] = 'a';
	va[1] = 'p';
	va[2] = 'p';
	va[3] = 'l';
	va[4] = 'e';
	va[5] = '\0'; //important
	kprintf("[va]: 0x%x -> %s\n", va, va);
	// Should return SYSERR since exceeding the lower bound
	kprintf("Return value: %s\n", ((vmhfreemem((char *)0xFFFFFF, 1) == SYSERR) ? "SYSERR" : "OK"));
	// Should return SYSERR since exceeding the upper bound
	kprintf("Return value: %s\n", ((vmhfreemem((char *)0x1400000, 1) == SYSERR) ? "SYSERR" : "OK"));
	// Should return OK (1 byte before the upper bound)
	kprintf("Return value: %s\n", ((vmhfreemem((char *)0x13FFFFF, 1) == SYSERR) ? "SYSERR" : "OK"));
*/

/* A.3. */
/*

	kprintf("Return value: %s\n", ((vmhfreemem((char *)0x1000000, 1) == SYSERR) ? "SYSERR" : "OK"));
*/

/* A.4. */
/*
	char *va = vmhgetmem(1024);
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)va);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	va[0] = 'a';
	va[1] = 'p';
	va[2] = 'p';
	va[3] = 'l';
	va[4] = 'e';
	va[5] = '\0'; //important
	kprintf("[va]: 0x%x -> %s\n", va, va);
	// Should return SYSERR since exceeding the maximum allocated space
	kprintf("Return value: %s\n", ((vmhfreemem(va, 1025) == SYSERR) ? "SYSERR" : "OK"));
	//print_proc_einfo(currpid);
	// Should return OK
	kprintf("Return value: %s\n", ((vmhfreemem(va, 1024) == SYSERR) ? "SYSERR" : "OK"));
	//print_proc_einfo(currpid);
*/


/*
	B. ptable allocations tests:
		1. Exceed the number of available virtual pages
		2. Free and reallocate
		3. Same virtual address different frames (should print 'orange' then 'apple')
*/	

/* B.1. */
/*
	char *va;
	// Should return SYSERR
	kprintf("Return value: %s\n", (((va = vmhgetmem(-1)) == SYSERR) ? "SYSERR" : "OK"));
	// Should return SYSERR
	kprintf("Return value: %s\n", (((va = vmhgetmem(0)) == SYSERR) ? "SYSERR" : "OK"));
	// Should return SYSERR
	kprintf("Return value: %s\n", (((va = vmhgetmem(1025)) == SYSERR) ? "SYSERR" : "OK"));
	// Should return OK
	kprintf("Return value: %s\n", (((va = vmhgetmem(1024)) == SYSERR) ? "SYSERR" : "OK"));
	//print_proc_einfo(currpid);
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)va);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	va[0] = 'a';
	va[1] = 'p';
	va[2] = 'p';
	va[3] = 'l';
	va[4] = 'e';
	va[5] = '\0'; //important
	kprintf("[va]: 0x%x -> %s\n", va, va);
	// Should return OK
	kprintf("Return value: %s\n", ((vmhfreemem(va, 1024) == SYSERR) ? "SYSERR" : "OK"));
	//print_proc_einfo(currpid);
*/

/* B.2. */
/*
	char *va;
	// Should return OK
	kprintf("Return value: %s\n", (((va = vmhgetmem(500)) == SYSERR) ? "SYSERR" : "OK"));
	// Should return SYSERR
	kprintf("Return value: %s\n", (((va = vmhgetmem(525)) == SYSERR) ? "SYSERR" : "OK"));
	print_proc_einfo(currpid);
	// Should return OK
	kprintf("Return value: %s\n", (((va = vmhgetmem(524)) == SYSERR) ? "SYSERR" : "OK"));
	print_proc_einfo(currpid);
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)va);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	va[0] = 'a';
	va[1] = 'p';
	va[2] = 'p';
	va[3] = 'l';
	va[4] = 'e';
	va[5] = '\0'; //important
	kprintf("[va]: 0x%x -> %s\n", va, va);
	// Should return OK
	kprintf("Return value: %s\n", ((vmhfreemem(va, 500) == SYSERR) ? "SYSERR" : "OK"));
	print_proc_einfo(currpid);
*/

/* B.3. */
/*
	// Should return OK
	kprintf("Return value: %s\n", (((gva = vmhgetmem(1)) == SYSERR) ? "SYSERR" : "OK"));
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)gva);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)gva, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	gva[0] = 'a';
	gva[1] = 'p';
	gva[2] = 'p';
	gva[3] = 'l';
	gva[4] = 'e';
	gva[5] = '\0'; //important
	resume(create(sndA, 1024, 20, "sndA", 0, NULL));
	sleep(1);
	kprintf("Main [gva]: 0x%x -> %s\n", gva, gva);
	//print_proc_einfo(currpid);
*/
	return OK;
}


void sndA(void){

	kprintf("SndA\n");

	// Should return OK
	kprintf("Return value: %s\n", (((gva = vmhgetmem(1)) == SYSERR) ? "SYSERR" : "OK"));
	// Simulate page handler part [begin]
	uint32 page_number = get_pte((uint32)gva);
	alloc_e1table_entry(currpid, page_number);
	set_p_pte(currpid, (uint32)gva, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
	// Simulate page handler part [end]
	gva[0] = 'o';
	gva[1] = 'r';
	gva[2] = 'a';
	gva[3] = 'n';
	gva[4] = 'g';
	gva[5] = 'e';
	gva[6] = '\0'; //important
	kprintf("SndA [gva]: 0x%x -> %s\n", gva, gva);
	//print_proc_einfo(currpid);
}