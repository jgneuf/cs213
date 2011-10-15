Student Name: Jonathan Neufeld
Student ID: 30671093
CS ID: p9d8

1) Both programs make a call to ping from foo. static-call-reg does this without
pushing the return address onto the stack. The compiler knows ping does not make
any function calls, it is a leaf procedure, and simply stores the return address
in r6. ping's execution is simply a return to r6.

static-call-stack uses memory at addresses 0x1000 to 0x1024 to model the stack.
In foo, we initialize the stack pointer in r5 and set the return address in r6
in preparation for a function call to ping. ping has a prologue and epilogue.
Since there are no local variables and no arguments, the prologue simply pushes
the return address onto the stack (by loading r6 into memory at r5). Since there
is no body, the epilogue follows. ping loads the return address into r6 by
reading memory at r5 (the stack pointer). It then decrements the stack pointer
in preparation for the return. Then we simply jump to the return address.

If the compiler can detect leaf procedures we can avoid extraneous memory reads
and writes. static-call-reg.s uses registers and doesn't need to read/write to
memory, which is going to be quite fast. static-call-stack.s uses two read/write
operations in ping and has a push and pop operation for the return address. This
will be slower. On the other hand, if we have a non-leaf procedure, using the 
stack is necessary, especially in recursive calls. Also, if we had more arguments
and local variables we would have to store them on the stack before calling
another function. The register program couldn't do this, we would run out of
registers.

2) S8 begins in the mysterious start function by initializing the stack pointer
and storing a return address to start in preparation for the function foo. foo's
prologue begins by pushing the return address r6 onto the stack. The body grabs
the return address in preparation for a call to b and stores it in r6. Control
now moves to b. The prologue of b pushes the return address in r6 onto the
stack. b then pushes two local variables onto the stack, r0 = 0 and r1 = 1.
After the body, the epilogue pops twice by decrementing the stack pointer twice,
so it now points to the return address (we removed the two local variables).
The return address is now popped and loaded into r6. We jump to r6, which is
is the calling function foo. We are now in foo's epilogue, which pops its
return address off the stack and loads it into r6 (which is the return address
of the start function). We now jump to start which halts.

3) args-regs.s adds two numbers without extraneous stack usage; the compiler
knows add is a leaf procedure. start sets the stack pointer and stores its 
return address in r6. foo pushes that return address on the stack in the 
prologue, saving it for the epilogue's return to start. The body then loads
two numbers into registers r0 and r1. foo then stores its return address in
r6 in preperation for a call to add. add stores the result of an add operation
in r0 and jumps back to the return address, r6, which is foo. foo then address
of s into r1 and stores the return value of add (r0) into r1, so s = add(1,2).
The epilogue pops the return address off the stack and loads it into r6, then
returns to start.

args-stack.s differs in that in foo's body, the arguments to add are pushed
onto the stack one at a time. r0 = 2, then r0 is pushed onto the stack. Then
we set r0, the first argument to add, as the value just pushed onto the stack.
r1 = 1 and is pushed onto the stack, then r1 is set to the value just pushed
onto the stack as before. foo then sets its return address in preperation for
a call to add, then jumps to add. add is known to be a leaf procedure so
nothing is pushed onto the stack. Instead, knowing arg0 and arg1 are the last
two elements on the stack, we read those two elements from the stack, add
them and store the result in r0. Since r6 is still the return address, we
simply return. Now back in foo's body we pop the two arugments off the stack
and store the return value (r0) in s's address (i.e. s = add(1,2)). Now we
pop the return address off the stack and store it in r6, then jump to r6
which is start.

4) The cpy.s program executes as expected, by taking some input (here the src
array at 0x900) and copying its values into a local array in copy() called
dst. copy() expects two elements, however, where the second is the value 0.
Note that cpy_attack.s is the actual copy program but with my virus in src.
My virus is simply a different input. The first two elements are non-zero so
copy() doesn't terminate. There is no array bounds checking, so the program
will continue to copy values from src. dst is a local array, pushed onto the
stack at runtime. Before dst is allocated, the address copy() returns to is
pushed onto the stack. My input takes advantage of this by providing a new
return address as the next element of the array. I then provide 0 so the 
while loop terminates. The stack then discards the two elements of dst by
adjusting the stack pointer back to the return address -- which I've altered.
The new return address is some position in memory where my virus begins. The
virus then writes the value -1 into reach register. The only trick here is to
write the code for the virus and look at the hex values for each instruction.
My virus halts without returning control to the other calling function(s).

5) A5-a.s finds the largest element in an int array. A5-b.s has two functions,
foo and bar (for ease of use). foo takes two arguments, bar calls foo. There is
a global array arr created dynamically, and a pointer to it created locally in
foo each time the function is called. The program calls foo twice from bar, 
and the crux of the program is as follows:

    arr[arg1] = arr[arg1] + arg0;

So foo(1,2) sets arr[2] to arr[1] + 2. In this case, arr[2] = 1, since the
value at arr[2] is 0.

More details for my implementation are in the *.c files themselves.

