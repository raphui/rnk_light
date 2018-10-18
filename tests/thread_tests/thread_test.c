#include <kernel/thread.h>
#include <kernel/ktime.h>

#define printf(__VA_ARGS__)

void thread_a(void)
{
	printf("starting thread A\n");

	while (1) {
		printf("A\n");
		ktime_usleep(30000);
	}
}

void thread_b(void)
{
	printf("starting thread B\n");

	printf("B\n");
	ktime_usleep(10000);
}

void thread_c(void)
{
	printf("starting thread C\n");

	while (1) {
		printf("C\n");
		ktime_usleep(3000);
	}
}

void test(void)
{
	printf("Starting thread tests\n");

	printf("- adding thread A\n");
	add_thread(&thread_a, NULL, 4, 0);

	printf("- adding thread B\n");
	add_thread(&thread_b, NULL, 3, 0);

	printf("- adding thread C\n");
	add_thread(&thread_c, NULL, 2, 0);
}
