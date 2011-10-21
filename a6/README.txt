Student Name: Jonathan Neufeld
Student ID: 30671093
CS ID: p9d8

3) The test file tests.s tests only the new indirect jump instructions. I have
tested base + offset with no offset and with an offset as well as index with
various increasing indexes. All tests seem to be working fine.

4) The action in snippet A begins in bar. The return address to start is pushed
onto the stack, then room for an argument to a function call is pushed. A struct
pointer a is set to point to anA, then anA is stored on the stack as arg0. We store
the return address in r6 then jump to foo. Before analyzing foo, lets note that after
returning, arg0 is discarded (the struct anA) and a new struct pointer b is set to
point to aB, which is then pushed onto the stack as arg0. Pushed onto the stack as 
arg0 for another call to foo. bar calls foo with both a struct of type anA and 
a struct of type aB.

The creation of those structs is a function, new_A or new_B, that sets the
function pointers of each struct. struct anA, for example, has function pointers
ping and pong that point to A_ping and A_pong, respectively. struct aB has
function pointers ping, pong and wiff. When new_B is called, ping is set to
point to B_ping, wiff is set to point to B_wiff, but pong is set to point to
A_pong -- one of anA's functions in a sense.

foo stores the return address on the stack so it can make two function calls of
its own, each time saving its return address in r6. arg0 is a pointer to the 
actual struct, structs anA and aB. Both structs contain a jump table to functions.
This is a C workaround to polymorphic dispatch like Java's polymorphism. The
first call foo makes is to the first function in the struct's jump table, the
one with offset zero. In anA's call, this is to A_ping, since anA is at 0x1000
and offset 0 points to 0x400. The second call is to 0x1000 offset four, which
is A_pong. When foo is called with b at 0x1008, offset zero calls 0x402 which
is B_ping. The second call, with offset 4, is to A_pong.  

In snippet B we check if the given number i is less than 20. If it is, we jump
to the default case. Then we check if i is greater than 23, and again if it is
we jump to the default case. If i - 19 is zero to smaller, then i is 19 or
smaller and out of range, hence the jump to default. Likewise, if i - 23 is
greater than zero we know is is greater than 23 and is out of range, so we
jump to the default case. 

When i passes both these tests we know it is in the range we want. We load -20
into a register and add i with -20. We know i is in [20..23] so the result will
be in [0..3]. Using a jump table at 0x800 which stores the address to jump to
for each case. For example, if the result of i - 20 was 1, we could use a double
indirect indexed jump with offset one to jump to the correct case by using the
base address of jmptable and offset 1.

The execution of this program uses an offset of 2. The double indirect indexed
jump to jmptable (0x800) offset 2 (2 * 4) gives address 0x150, which is the case
for 22.

Each case trivially loads a constant value into r1 and jumps to the done case.
done stores the value in r1 to memory at j. If the default case is executed, it
simply loads 14 into r1. The other cases load i - 10 into r1, but they use hard-
coded values.

The key is in storing the jumptable in memory and using a double indirect jump.

5) A5-c.s does...

My program, A5-c.c...
