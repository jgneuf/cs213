#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"

#define NUM_ITERATIONS 1000

/* Struct holding a computation. */
struct Triple
{
		int arg0;
		int arg1;
};

/* Procedure for adding args of a Triple struct. */
void *
add ( void *ts )
{
		struct Triple *x = (struct Triple*) ts;
		int *result = (int *) malloc(sizeof ( int ) ); 
		*result = x->arg0 + x->arg1;
		return (void *) result;
}

/* Procedure for subtracting args of a Triple struct. */
void *
sub ( void *ts )
{
		struct Triple *x = (struct Triple*) ts;
		int *result = (int *) malloc( sizeof( int ) ); 
		*result = x->arg0 - x->arg1;
		return (void *) result;
}

/* Main: Compute ((1 + 2) - (3 + 4)) + 7 with threads. */
int 
main ( int* argc, char** argv ) 
{
		uthread_init( 2 );

		/* Create structs for (1 + 2) and (3 + 4). */
		struct Triple *t1 = (struct Triple*) malloc( sizeof( struct Triple ) );
		t1->arg0 = 1;
		t1->arg1 = 2;
		struct Triple *t2 = (struct Triple*) malloc( sizeof( struct Triple ) );
		t2->arg0 = 3;
		t2->arg1 = 4;

		/* Create threads for (1 + 2) and (3 + 4). */
		uthread_t *t1_thread, *t2_thread;
		t1_thread = uthread_create( add, (void*) t1 );
		t2_thread = uthread_create( add, (void*) t2 );

		/* Execute (1 + 2) and (3 + 4) and store results. */
		int r1 = *( (int *) uthread_join( t1_thread ) );
		int r2 = *( (int *) uthread_join( t2_thread ) );

		/* Create struct for (r1 - r2). */
		struct Triple *t3 = (struct Triple*) malloc( sizeof( struct Triple ) );
		t3->arg0 = r1;
		t3->arg1 = r2;

		/* Create thread for (r1 - r2). */
		uthread_t *t3_thread = uthread_create ( sub, (void *) t3 );
		int r3 = *( (int *) uthread_join( t3_thread ) );

		/* Create struct for (r3 + 7). */
		struct Triple *t4 = (struct Triple*) malloc( sizeof( struct Triple ) );
		t4->arg0 = r3;
		t4->arg1 = 7;

		/* Create thread for (r3 + 7). */
		uthread_t *t4_thread = uthread_create ( add, (void *) t4 );
		int r4 = *( (int *) uthread_join( t4_thread ) );
		
		printf( "%d\n", r4 );
}

