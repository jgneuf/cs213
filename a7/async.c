#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/* Comp struct holds an event that has been placed in the completion queue and
 * is waiting for an interrupt. When the interrupt arrives, the handler function
 * is called and is given the appropriate data, here buf and size.  */
struct Comp {
    void ( *handler ) ( void*, int );
    void* buf;
    int   siz;
};

/* A Triple struct houses two variables and a handler function.  */
struct Triple {
    int result;
    int arg0;
    int arg1;
    int slocked; // 1 if locked, 0 if unlocked
};

/* Function handler that takes a triple and adds its two variables, storing the
 * result in RSLT variable.  */
void add ( void *xvp, int n )
{
    struct Triple *xp = (struct Triple *) xvp;
    xp->result = xp->arg0 + xp->arg1;

    printf ( "%d + %d = %d\n", xp->arg0, xp->arg1, xp->result );
    fflush ( stdout );
}

/* Function handler that takes a triple and subtracts its two variables, storing
 * the result in RSLT.  */
void sub ( void *xvp, int n )
{
    struct Triple *xp = (struct Triple *) xvp;
    xp->result = xp->arg0 - xp->arg1;

    printf ( "%d + %d = %d\n", xp->arg0, xp->arg1, xp->result );
    fflush ( stdout );
}

/* Maintain a global queue of Comp structs. When the disk finishes reading, call
 * the handler function and dequeue the struct.  */
struct Comp comp[1000];
int    compHead = 0;
int    compTail = 0;

/* Enqueue a task by providing a function handler. This way we can continue 
 * computing while the disk does its thing (slowly).  */
void doAsync ( void (*aHandler) (void*, int), void* aBuf, int aSiz ) 
{
    compHead = (compHead + 1) % (sizeof (comp) / sizeof (struct Comp));
    comp[compHead].handler = aHandler;
    comp[compHead].buf     = aBuf;
    comp[compHead].siz     = aSiz;
}
 
/* An interrupt signal means we dequeue a task and call its handler function.  */
void interruptServiceRoutine () 
{
    struct Comp c;
    if ( compHead != compTail ) 
    {
        compTail = (compTail + 1) % (sizeof (comp) / sizeof (struct Comp));
        c = comp[compTail];
        c.handler (c.buf, c.siz);
    }
}

/* An example handler function that simply prints a string (maybe it's reading
 * it from disk?).  */
void printString ( void* buf, int siz ) 
{
    printf ( "%s\n", (char*) buf); 
    fflush ( stdout );
}

/* Startup function, get the OS to send signals every second and handle these
 * signals with our interupt function.  */
void boot () 
{
    signal ( SIGALRM, interruptServiceRoutine );
    ualarm ( 999999, 999999 );
}

/* Global result of the program for adding/subtracting.  */
int global_result = 0;

/* Run the program by first making sure the OS sends signals every second. Then
 * add some tasks that simulate disk reads, i.e. print a few strings. */
int main ( int argc, char** argv ) 
{
    boot ();
    printf ( "Scheduling some things to do asynchronously\n" ); 
    fflush ( stdout );

    /* Make a few triples. */
    struct Triple t1;
    t1.result = 0;
    t1.arg0   = 1;
    t1.arg1   = 2;
    
    struct Triple t2;
    t2.result = 0;
    t2.arg0   = 3;
    t2.arg1   = 4;

    struct Triple t3;
    t3.result = 0;
    t3.arg0   = t1.result;
    t3.arg1   = t2.result;

    struct Triple t4;
    t4.result = 0;
    t4.arg0   = t3.result;
    t4.arg1   = 7;

    doAsync ( add, &t1, 0 );
    doAsync ( add, &t2, 0 );
    doAsync ( sub, &t3, 0 );
    doAsync ( add, &t4, 0 );

    //printString ( "Thing 1", 0 );
    //printString ( "Thing 2", 0 );
    //printString ( "Thing 3", 0 );

    //doAsync ( printString, "Thing 1", 0 );
    //doAsync ( printString, "Thing 2", 0 );
    //doAsync ( printString, "Thing 3", 0 );

    printf ( "Now waiting for them to complete\n" ); 
    fflush ( stdout );
    while ( 1 );
}

