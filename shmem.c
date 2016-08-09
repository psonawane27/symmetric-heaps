#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "memalloc.h"
#include "dlmalloc.h"
#include "shmem.h"
#include <memkind.h>
#include <hbwmalloc.h>

#define SIZE 32768
#define NHEAPS 2

struct heap_info {
    
    void *heap_base;
    size_t heap_length;
    mspace heap_mspace;
    char *name;
    memkind_t kind;

} *heaps_ptr[NHEAPS];

void
shmem_init() {
    
    int i;
    int err = -1;
    
    /*
    size_t size = 2048;
    size_t t, f;
    memkind_t k;
    memkind_get_kind_by_name( "memkind_default", &k );
    memkind_get_size(k, &t, &f );
    printf( "Initial: free %zu total %zu kind %p\n", f, t, k );
    void *addr = memkind_malloc( k, size );
    size_t temp = f;
    memkind_get_size( k, &t, &f);
    temp = temp - f;
    printf( "After malloc: free %zu, tot %zu kind %p addr %p allocate = %zu\n", f, t, k, addr, temp );
    memkind_free( k, addr);
    memkind_get_size( k, &t, &f);
    printf( "After free: free %zu tot %zu kind %p\n",f, t, k );
    */

    for ( i = 0; i < NHEAPS; i++ ) {

        heaps_ptr[i] = (struct heap_info *) malloc( sizeof ( struct heap_info ) );
        
        if ( heaps_ptr[i] == NULL ) {
            
            err = 1;

        } else {

            if( hbw_check_available() == 0 && i%2 == 0 ) {
                
                heaps_ptr[i] -> name = "memkind_hbw";
                memkind_get_kind_by_name( heaps_ptr[i] -> name, &heaps_ptr[i] -> kind );
                
                printf( "Hbw memory detected.\nAllocating heap %d on hbw memory.\n", i );
           
           } else {

                heaps_ptr[i] -> name = "memkind_hbw";                         
                memkind_get_kind_by_name( heaps_ptr[i] -> name, &heaps_ptr[i] -> kind );

                printf( "Allocating heap %d  on standard memory.\n", i );

           }
           if ( heaps_ptr[i] -> kind == (void *) NULL) {

                err = 1;

            } else {
                
                size_t total, available;
                memkind_get_size( heaps_ptr[i] -> kind, &total, &available );
                heaps_ptr[i] -> heap_length = SIZE;
                printf( "Heap %d, kind: %p total: %zu free: %zu\n" ,
                            i, heaps_ptr[i] -> kind, total, available );

                heaps_ptr[i] -> heap_base = memkind_malloc( heaps_ptr[i] -> kind,       
                    heaps_ptr[i] -> heap_length);
            
                memkind_get_size( heaps_ptr[i] -> kind, &total, &available );

                printf( "After memkind_malloc Heap %d kind: %p total: %zu free: %zu\n" ,
                            i, heaps_ptr[i] -> kind, total, available );

                /*printf( "Memkind kind: %p and addr: %p\n", kind, 
                    heaps_ptr[i] -> heap_base );*/
            
                //heaps_ptr[i] -> heap_base =
                //    (void *) malloc (heaps_ptr[i] -> heap_length);  
    
                if ( heaps_ptr[i] -> heap_base == NULL ) {
            
                    err = 1;

                } else {
                    heaps_ptr[i] -> heap_mspace = shmemi_mem_init( heaps_ptr[i] -> heap_base, 
                        heaps_ptr[i] -> heap_length);
        
                    if ( heaps_ptr[i] -> heap_mspace == NULL ) {            

                        err = 1;
        
                    }   
                }
            }
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
        
        return ( void * )NULL;
        
    } else {
        
        size_t total, available;
        memkind_get_size( heaps_ptr[index] -> kind, &total, &available );
        printf( "Allocate %zu bytes in heap %d kind: %p total: %zu free: %zu\n" , 
            size, index, heaps_ptr[index] -> kind, total, available );
          
        orig = shmemi_mem_alloc(size, heaps_ptr[index] -> heap_mspace);
       
        /* printf( "Orig = %p, base = %p, end = %p, mspace = %p\n", 
            orig, heaps_ptr[index] -> heap_base , 
            heaps_ptr[index] -> heap_base + heaps_ptr[index] -> heap_length, 
            heaps_ptr[index] -> heap_mspace); */
        
        memkind_get_size( heaps_ptr[index] -> kind, &total, &available );

        printf( "After allocating %zu bytes in heap %d kind: %p total: %zu free: %zu\n" ,
                    size, index, heaps_ptr[index] -> kind, total, available );

        if ( orig > heaps_ptr[index] -> heap_base &&
            ( char * )orig < ( ( char * )heaps_ptr[index] -> heap_base + heaps_ptr[index] -> heap_length )) {
            
            return orig;
        
        } else {
            
            shmemi_mem_free ( orig, heaps_ptr[index] -> heap_mspace ); 
            free( orig );
            
            return ( void * )NULL;
        }

    }

}

int
getHeapIndex (void *addr) {
    int index, i;
    
    index = -1;

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
    size_t total, available;
    for( i=0; i<NHEAPS; i++) {
        //free ( heaps_ptr[i] -> heap_base );
        
        memkind_get_size( heaps_ptr[i] -> kind, &total, &available );
        printf("Freeing: kind %p, mem base %p free: %zu\n", heaps_ptr[i] -> kind, 
            heaps_ptr[i] -> heap_base, available );
        
        memkind_free( heaps_ptr[i] -> kind, heaps_ptr[i] -> heap_base );
        
        memkind_get_size( heaps_ptr[i] -> kind, &total, &available );
        printf( "After freeing avialable: %zu\n", available );
        //free ( heaps_ptr[i] -> heap_mspace );
        free ( heaps_ptr[i] );

    }
   
}

