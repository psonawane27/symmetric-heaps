#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "shmem.h"


int 
main( void ) {

    int i;
    int *heapVar1;
    double *heapVar2;
    int *listInt;
    float *listFloat;
    int N1 = 2048, N2 = 4096;

    shmem_init();  
    
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

    shmem_free ( heapVar1 );
    shmem_free ( heapVar2 );
    shmem_free ( listInt );
    shmem_free ( listFloat );
    
    return 0;
}
