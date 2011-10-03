Student Name: 	Jonathan Neufeld
Student ID: 	30671093
CS ID: 			p9d8

4) I'm not sure how to load a negative number into a register to test
shr but the logic looks right. Everything else should be working, all
instructions are tested.

5) Load starting address of d0 into register r0 and value of d0.f into
r1. Then store value of r1 (d0.f) into memory at address r0, which is
d0.e. Second bit is slightly more complicated because d1 is a pointer
and the struct it points to is dynamically allocated at runtime. Still,
it does basically the same thing. r0 stores address of d1, struct d1
points to, then value of d1 (i.e. d1->e) is loaded into r0. d1->f is
loaded into register r1. The value at r1 (i.e. d1->f) is now stored at
memory address r0 (no offset), which is d1->e.

7) I tested the program by comparing my implimentation with the solution
for i = [0..2]. I can't seem to get the load index instruction to work
from my assembly file, however, which means I'm just testing i = 0. The
commented out lines show what I tried. (Un)fortunately, the solution
simulator didn't handle my assembly code with said instruction any better
than my implementation did, so it's just my lack of understanding on the
SM213 language. Everything SHOULD work, though, including accessing an
index outside the bounds of the array with i = 2.

8)
a) A single memory read is needed to set v to s.x[i]. 
b) Two memory reads are needed to set v to s.y[i], one for the pointer
and another for the value pointed to.
c) Two memory reads are needed to set v to z->x[i], one for the pointer
and another for the value pointed to. 

10) 
FIXES IN TEST 1 and 2: ipc and ip are two pointers that point to the
same memory address. ip is deallocated, signalling the memory is now
free to be used later. That memory is used to push "Zero" onto the
stack. ipc and ip still hold the same memory address, though, so in
the for loop at the end of test 2 the address where "Zero" was pushed
onto the stack is written over. Here's a verbose example output:

	test2:
	:: Allocated ip and ipc
	&ip  = 136480l
	&ipc = 136480l
	:: De-allocated ip
	&ip  = 136480l
	&ipc = 136480l
	:: Pushed 'zero'
	&stackTop = 136480l
	
The fix is simply to remove ip and ipc.

FIXES IN TEST 3/4: The bug in these tests is almost exactly the same.
Memory from ip is freed but ipc still stores the address. "One" is
then pushed onto the stack at the memory address just freed by ip.
The loop then writes over the memory address. Here is a verbose example:

	:: Pushed 'zero'
	&stackTop = 136576l
	:: Allocated ip and ipc
	&ip  = 136688l
	&ipc = 136688l
	:: De-allocated ip
	&ip  = 136688l
	&ipc = 136688l
	:: Pushed 'one'
	&stackTop = 136688l
	
A segmentation fault occurs when we attempt a second pop operation.
The fix is to remove the ip and ipc pointers.

FIXES IN TEST 5/6: The array,
	char *x[2];
in test 6 stores two pointers to character arrays. After "zero" and
"one" are pushed onto the stack they are set to point to the
character arrays that were pushed, i.e. "one" and "zero". Those stack
elements are popped, freeing their memory. However, the pointers don't
store the character arrays, the store the address of those character
arrays in memory -- memory allocated for the structs. That memory is
free. When "two" and "three" are pushed onto the stack they are allocated
the next available free memory, which is where "zero" and "one" were.
Since they are the same size and type in memory, the char arrays are
in the same place. That's why the lines,
	printf ("%s\n", x[0]);
    printf ("%s\n", x[1]);
print "two" and "three" rather than "zero" and "one". Here's a verbose
example output:

	test6:
	:: Pushed 'zero'
	&stackTop = 136464l

	:: Pushed 'one'
	&stackTop = 136576l

	:: Popped 'one' and 'zero'
	&stackTop = 136444l
	&x[0] = 136576l
	&x[1] = 136464l

	:: Pushed 'two'
	&stackTop = 136464l

	:: Pushed 'three'
	&stackTop = 136576l
	&x[0] = 136576l
	&x[1] = 136464l

The fix is to change the stack implementation. Rather than dynamically
allocating memory for the stack elements in the push function, each
function must manage the elements manually. Freeing memory is also 
removed from pop. This is preferable to using reference counters, but
both would work.