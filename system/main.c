/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

extern uint32 pd_n_addr;
char *gva;

void testE1E2();

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
	//uint32 page_number = get_pte((uint32)va);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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
	//uint32 page_number = get_pte((uint32)va);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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
	//uint32 page_number = get_pte((uint32)va);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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
	//uint32 page_number = get_pte((uint32)va);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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
	//uint32 page_number = get_pte((uint32)va);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)va, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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
	//uint32 page_number = get_pte((uint32)gva);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)gva, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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

/* Test segmentation fault */
/*
		resume(create(sndA, 1024, 20, "sndA", 0, NULL));
		kprintf("Continue\n");
*/

/* Test eviction */
/**/
	char *va = vmhgetmem(1024);
	
	int k;
	for (k=0; k<1024*4096; ++k){
		va[k] = 'm';
	}
	va[k-2] = 'e';
	va[k-1] = '\0';

	// resume(create(sndA, 1024, 21, "sndA", 0, NULL));

	// random stress test :: FAILED
	resume(create(testE1E2, 1024, 21, "testE1E20", 0, NULL));
	resume(create(testE1E2, 1024, 21, "testE1E21", 0, NULL));
	resume(create(testE1E2, 1024, 21, "testE1E22", 0, NULL));
	resume(create(testE1E2, 1024, 21, "testE1E23", 0, NULL));
	resume(create(testE1E2, 1024, 21, "testE1E24", 0, NULL));
	resume(create(testE1E2, 1024, 21, "testE1E25", 0, NULL));

	//print_e1table();
	//kprintf("[va]: 0x%x -> %s\n", va, va);
    //print_e2table();
    //print_proc_einfo(currpid);
    //kprintf("END\n");


/**/

	while(1){
		
	}

	return OK;
}


void sndA(void){

	kprintf("SndA\n");
/*
	// Should return OK
	kprintf("Return value: %s\n", (((gva = vmhgetmem(1)) == SYSERR) ? "SYSERR" : "OK"));
	// Simulate page handler part [begin]
	//uint32 page_number = get_pte((uint32)gva);
	//alloc_e1table_entry(currpid, page_number);
	//set_p_pte(currpid, (uint32)gva, (proctab[currpid].ptable[page_number].eentry->address + pt_lsb12));
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
*/

/* Test segmentation fault */
/*
		*((uint32 *) 0x8FFFF000) = 0x0;
*/

/* Test eviction */
/**/
	char *va = vmhgetmem(1024);
	
	int k;
	for (k=0; k<1024*4096; ++k){
		va[k] = 'a';
	}
	va[k-2] = 'e';
	va[k-1] = '\0';
	resume(create(sndB, 1024, 22, "sndB", 0, NULL));
	//kprintf("[va]: 0x%x -> %s\n", va, va);
	for (k=0; k<1024*4096; ++k){
		va[k] = 'a';
	}
	va[k-2] = 'e';
	va[k-1] = '\0';
	vmhfreemem(va, 1);
	sleep(20);
/**/
}


void testE1E2(void){
	// ah! failing this table because all of processes are freed in one go, these process then get  
	// some of their desired pages without freeing anyone of them resulting in a DEADLOCK where all 
	// processes are waiting for each other to free their pages
	kprintf("Test E1E2\n");
	char *va = vmhgetmem(1024);
	kprintf("[testE1E2] allocated virtual address space : 0x%x\n", va);
	
	int k;
	for (k=0; k<1024*4096; ++k){
		va[k] = 'a';
	}
	va[k-2] = 'e';
	va[k-1] = '\0';
	kprintf("[testE1E2] filled virtual address space\n");
	sleep(1);
	kprintf("[testE1E2] %d: going to free\n", currpid);
	vmhfreemem(va, 1024);
	kprintf("done with testE1E2\n");
}
void sndB(void){

	kprintf("SndB\n");

	char *va = vmhgetmem(25);
	
	int k;
	for (k=0; k<24*4096; ++k){
		va[k] = 'b';
	}
	va[k++] = 'x';
	kprintf("HERE\n");
	va[k] = '\0';
	// kprintf("[va]: 0x%x -> %s\n", va, va);

}