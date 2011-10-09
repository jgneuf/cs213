Student Name: Jonathan Neufeld
Student ID:   30671093
CS ID:        p9d8

4) All branch instructions are implemented and tested in atests2.s. Everything
seems to be working fully.

5)
    S5 and S5a:
S5a's C code looks like a for loop without the for loop; each iteration
is executed sequentially in the function foo. S5's C code is the actual
loop, summing the elements of the array a. S5a's assembly code simply loads
an index = 0 into r0 and address of a[0] into r1. The sum of each a[i] is
stored in r2 by adding each a[i]. S5's assembly code does the same thing   
using a loop. r4 is the essentially the number of iterations. At the beginning
of each loop, r4 is added to i (r0 moved into r5). If the result is 0, we've
executed our for loop in full and we're done (jump to end_loop) otherwise we
continue executing the contents of the loop.

    S6 and S7:
S6 sets a variable max to the larger of two variables a and b. a and b are
loaded into registers. b is copied into r2 and given a negative sign by bitwise
NOT and an increment. The result of adding a and (-b) is stored in r2. A branch
directions execution to store the appropriate variable as max. If a-b is strictly
greater than zero we know a is larger than b, so max = a. Otherwise max = b.

S7 stores program counter plus six (2 instructions later in this case) in r6. It
then performs an unconditional jump to ping at address 0x500. ping() then performs
an offset/register jump to r6 + 0x0, which is the halt instruction.

6) I divide my assembly program into labels. The loop is contained in the label
foo, which is sort of like the function foo. The label bar calls foo. Before any
of these execute, the variables are loaded into registers. The function foo has
a loop which is hard-coded to run 10 times, one for each element of the array. 
Since we have trivial branching instructions we subtract the total number of
iterations by the current index. If the result is zero, the loop is finished.
Now we load a[i] into a register and add it to -min. If the result is strictly
greater than 0 then min is smaller than a[i] so we branch to the beginning of
the loop. Otherwise a[i] is equal to or less than the current min value, so we
set min to a[i] and then continue the loop. Either way, before the potential
branch we increment the index i.


