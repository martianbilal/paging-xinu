#include <xinu.h>

syscall vmhfreemem(char *blockaddr, uint16 msize){
	// go to currprocess->pages
	// iterate and update them
		// follow their pointer and deallocate e1 entries also
		// if e1 entries exist update ptes also (zeroing them)
}