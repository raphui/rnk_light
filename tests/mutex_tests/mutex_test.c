#include <kernel/thread.h>
#include <kernel/ktime.h>
#include <kernel/kmutex.h>

static struct thread thr_a;
static struct thread thr_b;
static struct mutex mutex;

#define printf(__VA_ARGS__)

void thread_a(void)
{
	printf("starting thread A\n");

	while (1) {
		printf("[A] locking mutex\n");
		kmutex_lock(&mutex);

		ktime_usleep(30000);

		printf("[A] unlocking mutex\n");
		kmutex_unlock(&mutex);

		printf("[A] thread B should wake up after locking again....\n");
	}
}

void thread_b(void)
{
	printf("starting thread B\n");

	while (1) {
		printf("[B] unlocking thread A, shoud fail...\n");
		kmutex_unlock(&mutex);

		printf("[B] locking mutex\n");
		kmutex_lock(&mutex);

		printf("[B] unlocking mutex\n");
		kmutex_unlock(&mutex);
	}
}

void test(void)
{
	printf("Starting mutex tests\n");

	kmutex_init(&mutex);

	printf("- adding thread A\n");
	add_thread(&thread_a, NULL, 10, 1);

	printf("- adding thread B\n");
	add_thread(&thread_b, NULL, 2, 1);
}
