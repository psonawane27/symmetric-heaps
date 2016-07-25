#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "memalloc.h"
#include "dlmalloc.h"


#define SIZE 32768
#define NHEAPS 2

struct heap_info {
    
    void *heap_base;
    size_t heap_length;
    mspace heap_mspace;

} *heaps_ptr[NHEAPS];

void
heap_init() {
    
    int i;
    int err = -1;

    for ( i = 0; i < NHEAPS; i++ ) {

        heaps_ptr[i] = (struct heap_info *) malloc( sizeof ( struct heap_info ) );
        heaps_ptr[i] -> heap_length = SIZE;
        
        heaps_ptr[i] -> heap_base =
            (void *) malloc (heaps_ptr[i] -> heap_length);
        
        //printf( "Malloced addr for heap %d is: %p, capacity: %d\n", 
        //    i, heaps_ptr[i] -> heap_base, heaps_ptr[i] -> heap_length );

        heaps_ptr[i] -> heap_mspace = shmemi_mem_init( heaps_ptr[i] -> heap_base, 
            heaps_ptr[i] -> heap_length);
        
        //printf("heap_mspace for heap %d after init is : %p\n", i, heaps_ptr[i] -> heap_mspace);

        if ( heaps_ptr[i] -> heap_mspace == (void *)0 ) {            

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
        
        printf( "Allocate %d bytes in heap %d\n" , 
            size, index );
           
        orig = shmemi_mem_alloc(size, heaps_ptr[index] -> heap_mspace);
        
        return orig;

    }
        /*printf( "Before allocation, size of heap %d is: %d\n", 
            index, ( heaps_ptr[index] -> heap_curr - heaps_ptr[index] -> heap_base ) );*/ 
        
        //orig = heaps_ptr[index] -> heap_curr;

        /*heaps_ptr[index] -> heap_curr += size;

        if ( ( heaps_ptr[index] -> heap_curr - heaps_ptr[index] -> heap_base ) < 
            heaps_ptr[index] -> heap_length ){
    
            //heaps_ptr[index] -> heap_curr += size;
            
            orig = shmemi_mem_alloc(size, heaps_ptr[index] -> heap_mspace);
        
            printf( "After allocation, size of heap %d is: %d\n",
                        index, ( heaps_ptr[index] -> heap_curr - heaps_ptr[index] -> heap_base ) );
            return orig;
        
        } else{

            printf( "Error: Not enough memory in heap %d to allocate %d bytes.\n", 
                index, size );

            return NULL;
        
        }
    
    }
    
    for( i = 0; i < NHEAPS; i++ ){

        printf( "Available space in heap %d is %d\n" ,
            i, ( heaps_ptr[i] -> heap_length - ( heaps_ptr[i] -> heap_curr -
                heaps_ptr[i] -> heap_base ) ) );

    }*/

}


void
shmem_free ( void *addr, int index ) {
    
    shmemi_mem_free ( addr, heaps_ptr[index] -> heap_mspace );
}

int 
main( void ) {

    int i;
    int *heapVar1;
    double *heapVar2;
    int *listInt;
    float *listFloat;
    int N1 = 2048, N2 = 4096;

    heap_init();  
    
    heapVar1 = ( int* ) shmalloc ( sizeof (int), 0 );
    
    assert( heapVar1 != (void *)0 );

    *heapVar1 = 100;
    printf( "Value of variable at heap 0 is %d\t" , *heapVar1 );
    printf( "%s\n", *heapVar1 == 100 ? "Correct" : "Incorrect" ); 
    
    heapVar2 = ( double* ) shmalloc ( sizeof (double), 1 );
    
    assert( heapVar2 != (void *)0 ); 
    
    *heapVar2 = 10.10;
    printf( "Variable in heap 1 is %lf\t" , *heapVar2 );
    printf( "%s\n", *heapVar2 == 10.10 ? "Correct" : "Incorrect" );    

    listInt = (int* ) shmalloc ( N1 * sizeof (int), 0 );
    
    assert( listInt != (void *)0 );

    for ( i=0; i<N1; i++) {
        listInt[i] = i;
    }
    
    printf( "List element = %d\t", listInt[N1-1] );
    printf( "%s\n", listInt[ N1-1 ] == ( N1-1 ) ? "Correct" : "Incorrect");

    listFloat = (float* ) shmalloc ( N2 * sizeof (float), 1 );

    assert( listFloat != (void *)0 );

    for ( i=0; i<N2; i++) {
        listFloat[i] = i * 0.1;
    }


    printf( "List element = %f\t", listFloat[N2-1] );
    printf( "%s\n", listFloat[ N2-1 ] == (( N2-1 )* 0.1 )? "Correct" : "Incorrect");

    shmem_free ( heapVar1, 0 );
    shmem_free ( heapVar2, 1 );
    shmem_free ( listInt, 0 );
    shmem_free ( listFloat, 1);
    
    return 0;
}
