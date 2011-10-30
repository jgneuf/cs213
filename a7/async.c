#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/* When a spin_lock is set to UNLOCKED the RESULT variable in a struct Triple
 * can be used.  */
#define LOCKED    1
#define UNLOCKED  0

/* Holds an event that has been placed in the completion queue and is waiting 
 * for an interrupt. When the interrupt arrives, the handler functionis called 
 * and is given the appropriate data, here buf and siz.  */
struct Comp {
    void ( *handler ) ( void*, int );
    void* buf;
    int   siz;
};

/* A Triple houses two int arguments and a handler function that will store some
 * operation's result in RESULT. SPIN_LOCK is set to LOCKED if the computation has
 * not finished, and is then changed to UNLOCKED by the handler function. This is
 * the protocol that I'm using.  */
struct Triple {
    int result;
    int arg0;
    int arg1;
    int spin_lock; 
};

/* This function handler for Triples (so it had better be given a Triple) adds ARG0
 * and ARG1 and stores the result in RESULT. After computation, it sets SPIN_LOCK
 * to UNLOCKED in case any other Triples require this result.  */
void add ( void *xvp, int n )
{
    struct Triple *xp = (struct Triple *) xvp;
    xp->result    = xp->arg0 + xp->arg1;
    xp->spin_lock = UNLOCKED;
}

/* This function handler for Triples (so it had better be given a Triple) subtracts 
 * ARG0 and ARG1 and stores the result in RESULT. After computation, it sets SPIN_LOCK
 * to UNLOCKED in case any other Triples require this result.  */
void sub ( void *xvp, int n )
{
    struct Triple *xp = (struct Triple *) xvp;
    xp->result    = xp->arg0 - xp->arg1;
    xp->spin_lock = UNLOCKED;
}

/* Maintain a global queue of Comp structs. Assuming the tasks complete in order,
 * each time we receive SIGALARM from the OS we dequeue a task and use the handler
 * function on it, done by interruptServiceRoutine. This is to simulate disk reads.  */
struct Comp comp[1000];
int    compHead = 0;
int    compTail = 0;

/* Enqueue a task by providing a function handler. This way we can continue computing 
 * while the disk does its thing (slowly). When SIGALARM is caught by 
 * interruptServiceRoutine the task is dequeued and the handler function continues 
 * processing with the requested data, what would be a disk read. The queue is simple 
 * array that wraps from the last index to the first using the modulo operator. The 
 * head and tail are incremented when tasks are enqueued and dequeued.  */
void doAsync ( void (*aHandler) (void*, int), void* aBuf, int aSiz ) 
{
    compHead = (compHead + 1) % (sizeof (comp) / sizeof (struct Comp));
    comp[compHead].handler = aHandler;
    comp[compHead].buf     = aBuf;
    comp[compHead].siz     = aSiz;
}
 
/* This function is called by the OS to catch SIGALARM. This simulates an interrupt 
 * indicating the disk has finished reading; the handler function takes what was being
 * read by disk and completes whatever computation it ought to. We wouldn't want to 
 * wait for the disk to finish because it's slow, so we give a handler function to do 
 * the necessary work later, when the disk is ready. That way we can (hopefully) find 
 * something else to do in the mean time so we don't waste the CPU's ability.  */
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

/* An example handler function that simply prints a string (maybe it's reading the 
 * string from the disk).  */
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
 * add some tasks that simulate disk reads, i.e. print a few strings or evaluate an
 * algebraic expression.  */
int main ( int argc, char** argv ) 
{
    boot ();
    printf ( "Computing: (((1 + 2) - (3 + 4)) + 7)\n" );
    fflush ( stdout );

    /* Compute (((1 + 2) - (3 + 4)) + 7). We know the two innermost expressions
     * must be evaluated first, so we can put them into the queue first. We also
     * know they are required by further computations, so we place a lock on them
     * so they can't be used until their locks are released.  */
    struct Triple t1;
    t1.arg0 = 1;
    t1.arg1 = 2;
    t1.spin_lock = LOCKED;
    
    struct Triple t2;
    t2.arg0 = 3;
    t2.arg1 = 4;
    t2.spin_lock = LOCKED;

    doAsync ( add, &t1, 0 ); // Enqueue t1
    doAsync ( add, &t2, 0 ); // Enqueue t2

    /* The next two computations require the results of the previous two. We'll 
     * spin on their locks until the result is computed, then proceed.  */
    struct Triple t3;
    while ( t1.spin_lock != UNLOCKED ) {} // Spin on t1
    t3.arg0 = t1.result;
    while ( t2.spin_lock != UNLOCKED ) {} // Spin on t2
    t3.arg1 = t2.result;
    t3.spin_lock = LOCKED;
    doAsync ( sub, &t3, 0 ); // Enqueue t3

    struct Triple t4;
    while ( t3.spin_lock != UNLOCKED ) {} // Spin on t3
    t4.arg0 = t3.result;
    t4.arg1 = 7;
    t4.spin_lock = LOCKED;
    doAsync ( add, &t4, 0 ); // Enqueue t4

    /* Finally, capture the result of the expression in a global variable and
     * print it to the screen. We have to spin on this, too.  */
    while ( t4.spin_lock != UNLOCKED ) {}
    global_result = t4.result;
    printf ( "Result: %d\n", global_result );

    //printString ( "Thing 1", 0 );
    //printString ( "Thing 2", 0 );
    //printString ( "Thing 3", 0 );

    //doAsync ( printString, "Thing 1", 0 );
    //doAsync ( printString, "Thing 2", 0 );
    //doAsync ( printString, "Thing 3", 0 );

    //while ( 1 );
}

