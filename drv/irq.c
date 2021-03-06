#include <errno.h>
#include <string.h>
#include <mm/mm.h>
#include <drv/irq.h>
#include <armv7m/vector.h>
#include <init.h>
#include <trace/SEGGER_SYSVIEW_rnk.h>

int irq_action(void)
{
	int ret = 0;
	unsigned int irq;
	struct isr_entry *entry = NULL;

	trace_enter_isr();

	irq = vector_current_irq();

	entry = vector_get_isr_entry(irq);
	if (!entry)
		return -ENXIO;

	if (entry->isr)
		entry->isr(entry->arg);

	trace_exit_isr();

	return ret;	
}

int irq_request(unsigned int irq, void (*handler)(void *), void *arg)
{
	int ret = 0;
	struct isr_entry entry;

	entry.isr = handler;
	entry.arg = arg;

	ret = vector_set_isr_entry(&entry, irq);

	return ret;
}

int irq_init(void)
{
	int ret = 0;

	return ret;
}
coredevice_initcall(irq_init);
