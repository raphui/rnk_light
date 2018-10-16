#include <kernel/thread.h>
#include <kernel/ktime.h>
#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/spinlock.h>
#include <init.h>

static struct list_node sleeping_threads;

static void remove_sleeping_thread(struct thread *thread)
{
	list_delete(&thread->node);
}

int time_init(void)
{
	int ret = 0;

	list_initialize(&sleeping_threads);

	return ret;
}
core_initcall(time_init);

void ktime_usleep(unsigned int usec)
{
	struct thread *thread = NULL;

	thread = get_current_thread();
	thread->delay = (usec / 1000) + system_tick;

	thread->state = THREAD_BLOCKED;
	remove_runnable_thread(thread);

	list_add_tail(&sleeping_threads, &thread->node);

	schedule_yield();
}

void decrease_thread_delay(void)
{
	struct thread *thread;
	struct thread *tmp;
	struct thread *curr = get_current_thread();

	list_for_every_entry_safe(&sleeping_threads, thread, tmp, struct thread, node) {
		if (thread->delay <= system_tick) {
			remove_sleeping_thread(thread);
			insert_runnable_thread(thread);

#ifdef CONFIG_SCHEDULE_PRIORITY
			if (curr->priority < thread->priority)
				schedule_yield();
#endif /* CONFIG_SCHEDULE_PRIORITY */
		}
	}
}
