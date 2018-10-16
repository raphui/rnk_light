#ifndef MM_H
#define MM_H

#include <stddef.h>

extern int heap_size;
extern int mem_alloc;
#define mem_available (heap_size - mem_alloc)

extern int heap_init(void);
extern void *kmalloc(size_t size);
extern void kfree(void *mem);

#endif /* MM_H */
