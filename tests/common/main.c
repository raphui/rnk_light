#include <kernel/scheduler.h>
#include <kernel/thread.h>
#include <mm/mm.h>
#include <kernel/ktime.h>
#include <arch/system.h>
#include <arch/spinlock.h>
#include "rtt_printf/SEGGER_RTT.h"

#include <init.h>

extern initcall_t __rnk_initcalls_start[], __rnk_initcalls_end[];
extern exitcall_t __rnk_exitcalls_start[], __rnk_exitcalls_end[];
extern void test(void);

int main(void)
{
	unsigned long irqstate;
	initcall_t *initcall;

	arch_interrupt_save(&irqstate, SPIN_LOCK_FLAG_IRQ);

	arch_init();

	for (initcall = __rnk_initcalls_start; initcall < __rnk_initcalls_end; initcall++)
		(*initcall)();

	test();

	start_schedule();

	arch_interrupt_restore(irqstate, SPIN_LOCK_FLAG_IRQ);

	while(1)
		;

	return 0; //Never reach
}
