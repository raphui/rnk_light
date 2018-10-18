#include <kernel/thread.h>
#include <kernel/ksem.h>

#define printf(__VA_ARGS__)

static struct semaphore sem;

void thread_a(void)
{
	int times_wakeup = 0;

	printf("starting thread A\n");

	while (1) {
		printf("[A] waiting on sem\n");
		ksem_wait(&sem);

		if (!(++times_wakeup % 2)) {
			printf("[A] waking up thread B\n");
			ksem_post(&sem);

			printf("[A] should wakup after waiting again...\n");
		}
	}
}

void thread_b(void)
{
	printf("starting thread B\n");

	while (1) {
		printf("[B] waking up thread A\n");
		ksem_post(&sem);

		printf("[B] waiting on sem\n");
		ksem_wait(&sem);
	}
}

void thread_c(void)
{
	printf("starting thread C\n");

	while (1) {
		printf("[C] waking up thread A\n");
		ksem_post(&sem);
	}
}

void test(void)
{
	printf("Starting mutex tests\n");

	ksem_init(&sem, 1);

	printf("- adding thread A\n");
	add_thread(&thread_a, NULL, 4, 0);

	printf("- adding thread B\n");
	add_thread(&thread_b, NULL, 3, 0);

	printf("- adding thread C\n");
	add_thread(&thread_c, NULL, 2, 0);
}
