#include <init.h>
#include <mm/memory.h>

int heap_size = MAX_KERNEL_HEAP_SIZE;

int heap_init(void)
{
	int ret = 0;
	int i;

	for (i = 0; i < KERNEL_NUM_BLOCKS; i++) {
		kernel_heap[i].free_chunks = CHUNK_PER_BLOCK;
		kernel_heap[i].free_mask = 0;
	}

	return ret;
}
pure_initcall(heap_init);
