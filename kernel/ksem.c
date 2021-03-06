#include <kernel/ksem.h>
#include <kernel/wait.h>
#include <kernel/thread.h>
#include <errno.h>
#include <kernel/scheduler.h>
#include <kernel/spinlock.h>
#include <kernel/syscall.h>

int ksem_init(struct semaphore *sem, int value)
{
	int ret = 0;

	if (!sem) {
		ret = -EINVAL;
		goto err;
	}

	sem->value = value;
	sem->count = 0;

	wait_queue_init(&sem->wait);

err:
	return ret;
}

int ksem_wait(struct semaphore *sem)
{
	int ret = 0;
	unsigned long irqstate;

	arch_interrupt_save(&irqstate, SPIN_LOCK_FLAG_IRQ);

	if (!sem) {
		ret = -EINVAL;
		goto err;
	}

	if (--sem->count < 0)
		ret = wait_queue_block_irqstate(&sem->wait, &irqstate);
err:
	arch_interrupt_restore(irqstate, SPIN_LOCK_FLAG_IRQ);
	return ret;
}

int ksem_post(struct semaphore *sem)
{
	int ret = 0;
	unsigned long irqstate;

	arch_interrupt_save(&irqstate, SPIN_LOCK_FLAG_IRQ);

	if (!sem) {
		ret = -EINVAL;
		goto err;
	}

	sem->count++;

	if (sem->count > sem->value)
		sem->count = sem->value;

	if (sem->count <= 0)
		ret = wait_queue_wake_irqstate(&sem->wait, &irqstate);

err:
	arch_interrupt_restore(irqstate, SPIN_LOCK_FLAG_IRQ);
	return ret;
}
