#include <stdlib.h>
#include <stdio.h>
#include "memalloc.h"
#include "dlmalloc.h"
#include "shmem.h"

#define SIZE 32768
#define NHEAPS 2

struct heap_info {
    
    void *heap_base;
    size_t heap_length;
    mspace heap_mspace;

} *heaps_ptr[NHEAPS];

void
shmem_init() {
    
    int i;
    int err = -1;

    for ( i = 0; i < NHEAPS; i++ ) {

        heaps_ptr[i] = (struct heap_info *) malloc( sizeof ( struct heap_info ) );
        heaps_ptr[i] -> heap_length = SIZE;
        
        heaps_ptr[i] -> heap_base =
            (void *) malloc (heaps_ptr[i] -> heap_length);  
    

        heaps_ptr[i] -> heap_mspace = shmemi_mem_init( heaps_ptr[i] -> heap_base, 
            heaps_ptr[i] -> heap_length);
        
      
        if ( heaps_ptr[i] -> heap_mspace == NULL ) {            

            err = 1;
        
        }

    }

    if ( err == 1 ) {

        printf( "Error initializing heap!\n" );
    
    } else {
     
        printf( "Initialization completed successfully.\n" );
    
    }
}

void * 
shmalloc( size_t size, int index ) {

    void* orig;
   
    if ( index >= NHEAPS || index < 0 ) {
    
        printf( "Error: Heap index out of bound!\n" );
        
        return NULL;
        
    } else {
        
        printf( "Allocate %zu bytes in heap %d\n" , 
            size, index );
           
        orig = shmemi_mem_alloc(size, heaps_ptr[index] -> heap_mspace);
       
        /* printf( "Orig = %p, base = %p, end = %p, mspace = %p\n", 
            orig, heaps_ptr[index] -> heap_base , 
            heaps_ptr[index] -> heap_base + heaps_ptr[index] -> heap_length, 
            heaps_ptr[index] -> heap_mspace); */
        
        if ( orig > heaps_ptr[index] -> heap_base &&
            ( char * )orig < ( ( char * )heaps_ptr[index] -> heap_base + heaps_ptr[index] -> heap_length )) {
            
            return orig;
        
        } else {
            
            shmemi_mem_free ( orig, heaps_ptr[index] -> heap_mspace ); 
            
            return NULL;
        }

    }

}

int
getHeapIndex (void *addr) {
    int index, i;

    for( i=0; i<NHEAPS; i++) {
        if( ( addr > heaps_ptr[i] -> heap_base) && 
            ( ( char * )addr < ( ( char * )heaps_ptr[i] -> heap_base + heaps_ptr[i] -> heap_length ))) {
                index = i;
                break;
            }

    }

    return index;
}

void
shmem_free ( void *addr ) {
 
    int index;

    index = getHeapIndex( addr );
    printf( "Freeing memory from heap %d\n",index );

    shmemi_mem_free( addr, heaps_ptr[index] -> heap_mspace );
}

void
shmem_finalize() {
    
    int i;

    for( i=0; i<NHEAPS; i++) {
        free ( heaps_ptr[i] -> heap_base );
        free ( heaps_ptr[i] );

    }
}

