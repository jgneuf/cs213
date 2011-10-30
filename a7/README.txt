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

4) Modified async.c description.

