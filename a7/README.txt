Student Name: Jonathan Neufeld
Student ID: 30671093
CS ID: p9d8

3) The first "element" on the stack is printString. The two arguments to are 
the string to print (the address shown changes with each call, since a different
string is printed each time) and an int (siz, always 0). The calling function is
always interruptServiceRoutine (), and below that are <signal handler called> and
main. This makes sense, since boot tells the OS to use interruptServiceRoutine ()
each second when SIGALARM is sent to the program. interruptServiceRoutine () looks
at the Comp struct at the head of the queue, grabs the handler function and calls
it with the Comp struct's data -- buf and siz. On a higher level, each time the
OS sends a SIGALARM our program dequeues the next Comp struct. The three comp
structs we enqueued at the beginning of the program were the printString structs.
Enqueueing these happens extremely fast, so they are all enqueued before the first
interrupt.

In the version of async.c that calls printString direclty from main, the stack is
smaller in each case. As before, printString is on the top of the stack with the
string argument's address differing in each case. However, since printString is
called directly from main and not through an interrupt function, main is the only
other function on the stack. The calling address in main is different in each case
too, since it is called from seperate lines, whereas before the same line of code
in interruptServiceRoutine () called the handler.

4) My async.c defines a struct Triple that has ints ARG0 and ARG1 as well as a 
result int, RESULT. It also has a lock variable, SPIN_LOCK. The lock can be set to 
LOCKED if we know some other computation requires RESULT. The two handler functions,
ADD and SUB, will set SPIN_LOCK to UNLOCKED after computing RESULT. That way, if
some other Triple requires RESULT, it check the lock until the lock is set to
UNLOCKED.

I know the order (((1 + 2) - (3 + 4)) + 7) needs to be evaluated in, so I create my
Triples in a somewhat synchronized manner. t1 and t2, the innermost expressions,
don't require a result from any other Triple, so I enqueue them right away. What I
notice is their results are required by other expressions, so I set their locks to
LOCKED. This ensures RESULT cannot be accessed until the computation is complete.
t3, for example, requires the results of both t1 and t2. I spin on both RESULTs 
before I can set t3's data. t3 is also LOCKED since its result is required by t4.
The process of enqueueing and spinning is similar for t4. GLOBAL_VARIABLE also
spins on t4's result before it can print the result to the screen. The rest is well
documented in my code.

