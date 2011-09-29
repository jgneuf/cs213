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
SM213 language.

8)
a) A single memory read is needed to set v to s.x[i]. 
b) Two memory reads are needed to set v to s.y[i], one for the pointer
and another for the value pointed to.
c) Two memory reads are needed to set v to z->x[i], one for the pointer
and another for the value pointed to. 

10)