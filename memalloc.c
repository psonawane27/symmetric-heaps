
#include "dlmalloc.h"

mspace myspace;


void *
shmemi_mem_init (void *base, size_t capacity) {
  
    myspace = create_mspace_with_base (base, capacity, 1);
    printf("In init: mspace: %p, base: %p, capacity: %d\n", myspace, base, capacity);
    return myspace;
}

void *
shmemi_mem_alloc (size_t size, mspace heap_space) {
    
    void *addr = mspace_malloc (heap_space, size);

    return addr;
}

void
shmemi_mem_free (void *addr, mspace heap_space) {
    
    mspace_free (heap_space, addr);
}
