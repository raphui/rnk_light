#include <mm/memory.h>

static void free_mem(void *mem, struct memory_block *heap, void *base)
{
	struct alloc_header *header = (struct alloc_header *)((unsigned int *)mem - sizeof(struct alloc_header));
	unsigned int chunks = header->chunks;
	unsigned int index_block = addr_to_block((void *)header, base);
	unsigned int block_to_free;
	int i;

	if (header->magic != MAGIC)
		return;

	if (chunks < CHUNK_PER_BLOCK) {
		heap[index_block].free_mask &= ~(MASK(chunks) << addr_to_chunks_offset((void *)header, base));
		heap[index_block].free_chunks += chunks;
	} else {
		block_to_free = chunks / CHUNK_PER_BLOCK;
		chunks = chunks % CHUNK_PER_BLOCK;

		for (i = 0; i < block_to_free; i++) {
			heap[index_block + i].free_mask = 0;
			heap[index_block + i].free_chunks = CHUNK_PER_BLOCK;
		}

		heap[index_block + block_to_free].free_mask &= ~MASK(chunks);
		heap[index_block + block_to_free].free_chunks += chunks;
	}
}

void kfree(void *mem)
{
	free_mem(mem, kernel_heap, (void *)KERNEL_HEAP_START);
}
