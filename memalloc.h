#include <sys/types.h>
#include "dlmalloc.h"

extern void *shmemi_mem_init (void *base, size_t capacity);
//extern void shmemi_mem_finalize (void);
extern void *shmemi_mem_alloc (size_t size, mspace heap_space);
extern void shmemi_mem_free (void *addr, mspace heap_space);
