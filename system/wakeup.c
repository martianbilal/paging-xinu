/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	pid32 pid;			/* Stores ID of process to wakeup	*/
	/* Awaken all processes that have no more time to sleep */

	resched_cntl(DEFER_START);
	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
		pid = dequeue(sleepq);
		kprintf("wakeup: %d\n", pid);
		ready(pid);
	}

	resched_cntl(DEFER_STOP);
	return;
}
