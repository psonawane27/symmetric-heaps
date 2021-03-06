#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "shmem.h"

int 
main( void ) {

    int i;
    int *heapVar1;
    float *heapVar2;
    int *listInt;
    float *listFloat;
    int N1 = 2048, N2 = 4096;

    shmem_init();  
    
    heapVar1 = ( int* ) shmalloc ( sizeof (int), 0 );
    
    assert( heapVar1 != NULL );

    *heapVar1 = 100;
    printf( "Value of variable at heap 0 is %d\t" , *heapVar1 );
    printf( "%s\n", *heapVar1 == 100 ? "Correct" : "Incorrect" ); 
    
    heapVar2 = ( float* ) shmalloc ( sizeof (float), 1 );
    
    assert( heapVar2 != NULL ); 
    
    *heapVar2 = 10.10;
    printf( "Variable in heap 1 is %lf\t" , *heapVar2 );
    printf( "%s\n", fabs( *heapVar2 - ( float )10.10)  < 0.0001 ? "Correct" : "Incorrect" );    

    listInt = (int* ) shmalloc ( N1 * sizeof (int), 0 );
    
    assert( listInt != NULL );

    for ( i=0; i<N1; i++) {
        listInt[i] = i;
    }
    
    printf( "List element = %d\t", listInt[N1-1] );
    printf( "%s\n", listInt[ N1-1 ] == ( N1-1 ) ? "Correct" : "Incorrect");

    listFloat = (float* ) shmalloc ( N2 * sizeof (float), 1 );

    assert( listFloat != NULL );

    for ( i=0; i<N2; i++) {
        listFloat[i] = ( float )i * ( float )0.1;
    } 

    printf( "List element = %f\t", listFloat[N2-1] );
    printf( "%s\n", fabs(listFloat[ N2-1 ] - ( float )(( N2-1 )* 0.1 )) < 0.0001 ? "Correct" : "Incorrect");

    shmem_free ( heapVar1 );
    shmem_free ( heapVar2 );
    shmem_free ( listInt );
    shmem_free ( listFloat );
    
    shmem_finalize();
        
    return 0;
}
