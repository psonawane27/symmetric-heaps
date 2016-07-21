#include <stdio.h>
#include <assert.h>

#define SIZE 1024
#define NHEAPS 3

struct heap_info {
    
    void* heap_base;
    int heap_length;
    void* heap_curr;

} *heaps_ptr[NHEAPS];

void heap_init() {
    
    int i;
    int err = -1;
    
    for ( i = 0; i < NHEAPS; i++ ) {

        heaps_ptr[i] = malloc ( sizeof ( struct heap_info ) );
        heaps_ptr[i] -> heap_length = SIZE;
        
        heaps_ptr[i] -> heap_base =
            heaps_ptr[i] -> heap_curr =
            malloc(heaps_ptr[i] -> heap_length);

        if ( heaps_ptr[i] -> heap_base == NULL ) {            

            err = 1;
        
        }

    }

    if ( err == 1 ) {

        printf( "Error initializing heap!\n" );
    
    } else {
     
        printf( "Initialization completed successfully.\n" );
    
    }   
}

void* shmalloc( int size, int index ) {

    void* orig;
   
    if ( index >= NHEAPS || index < 0 ) {
    
        printf( "Error: Heap index out of bound!\n" );
        
        return NULL;
        
    } else {
        
        printf( "Allocate %d bytes in heap %d\n" , 
            size, index );
        
        printf( "Before allocation, size of heap %d is: %d\n", 
            index, ( heaps_ptr[index] -> heap_curr - heaps_ptr[index] -> heap_base ) ); 
        
        orig = heaps_ptr[index] -> heap_curr;

        heaps_ptr[index] -> heap_curr += size;

        if ( ( heaps_ptr[index] -> heap_curr - heaps_ptr[index] -> heap_base ) < 
            heaps_ptr[index] -> heap_length ){
    
            heaps_ptr[index] -> heap_curr += size;

            printf( "After allocation, size of heap %d is: %d\n",
                        index, ( heaps_ptr[index] -> heap_curr - heaps_ptr[index] -> heap_base ) );
            return orig;
        
        } else{

            printf( "Error: Not enough memory in heap %d to allocate %d bytes.\n", 
                index, size );

            return NULL;
        
        }
    
    }
    
    /*for( i = 0; i < NHEAPS; i++ ){

        printf( "Available space in heap %d is %d\n" ,
            i, ( heaps_ptr[i] -> heap_length - ( heaps_ptr[i] -> heap_curr -
                heaps_ptr[i] -> heap_base ) ) );

    }*/

}

int main( void ) {

    int i;
    int* heapVar;
    double* var2;

    heap_init();  
    
    heapVar = ( int* ) shmalloc ( sizeof (int), 0 );
    
    assert( heapVar != NULL );

    *heapVar = 100;
    printf( "Value of variable at heap 0 is %d\n" , *heapVar );

    var2 = ( double* ) shmalloc ( sizeof (double), 1 );
    
    assert( var2 != NULL ); 
    
    *var2 = 10.10;
    printf( "Variable in heap 1 is %lf\n" , *var2 );
    
    return 0;
}
