#include <stddef.h>
#include <string.h>
#include <mm/memory.h>

int mem_alloc = 0;

static void *ralloc(struct memory_block *heap, unsigned int heap_size, unsigned int chunks, void *base)
{
	void *ret = NULL;
	int i = 0;
	int j = 0;
	int off;
	unsigned int mask;
	unsigned free_blocks = 0;
	unsigned int blocks;
	struct alloc_header *header = NULL;

	if (chunks < CHUNK_PER_BLOCK) {
		mask = MASK(chunks);

		for (i = 0; i < heap_size; i++) {
			if (heap[i].free_chunks >= chunks) {
				for (off = 0; off <= (CHUNK_PER_BLOCK - chunks); off++) {
					if (is_free(heap[i].free_mask, (mask << off))) {
						heap[i].free_mask |= (mask << off);
						heap[i].free_chunks -= chunks;
						ret = to_addr(i, off, base);
						goto out;
					}
				}
			}
		}
	} else {
		blocks = chunks / CHUNK_PER_BLOCK;
		chunks = chunks % CHUNK_PER_BLOCK;
		mask = MASK(chunks);

		for (i = 0; i < heap_size; i++) {
			if (free_blocks < blocks) {
				if (!heap[i].free_mask)
					free_blocks++;
				else
					free_blocks = 0;
			} else if (free_blocks == blocks) {
				if (!(heap[i].free_mask & mask)) {
					for (j = 1; j < free_blocks; j++) {
						heap[i - j].free_mask = -1;
						heap[i - j].free_chunks = 0;
					}
					heap[i].free_mask |= mask;
					heap[i].free_chunks -= chunks;

					ret = to_addr(i - free_blocks, 0, base);
					goto out;
				}
			} else {
				free_blocks = 0;
			}
			
		}	
	}

out:
	if (!ret)
		return ret;

	header = (struct alloc_header *)ret;
	header->magic = MAGIC;
	header->chunks = chunks;
	ret = (void *)((unsigned int *)ret + sizeof(struct alloc_header));

	return ret;
}

static void alloc(size_t size, unsigned int *m)
{
	void *mem = NULL;
	int chunks;

	if (size > MAX_KERNEL_HEAP_SIZE)
		return NULL;

	size += sizeof(struct alloc_header);

	chunks = size + CHUNK_SIZE - 1;
	chunks = chunks / CHUNK_SIZE;

	mem = ralloc(kernel_heap, KERNEL_NUM_BLOCKS, chunks, (void *)KERNEL_HEAP_START);

	if (mem)
		mem_alloc += size;
	*m = (unsigned int)mem;
}

void *kmalloc(size_t size)
{
	unsigned int mem;

	alloc(size, &mem);

	return (void *)mem;
}
