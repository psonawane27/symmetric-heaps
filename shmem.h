#include <sys/types.h>

extern void shmem_init ();
extern void *shmalloc (size_t size, int index);
extern void shmem_free (void *addr);
