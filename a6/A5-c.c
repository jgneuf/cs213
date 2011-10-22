/* This can be compiled, because I feel strange submitting snippets of
 * actual C code. This is my translation of the SM213 program. My 
 * reasoning for this code is based on the idea that A5-c.s starts in 
 * some function that initializes the stack pointer before pushing three 
 * arguments onto the stack. Those three arguments are here arg0, arg1 
 * and arg2 for the mystery function, which I call foo. I've split this 
 * into functions, I've implemented the switch and I return a value 
 * where appropriate. My main function prints a value and my switch is 
 * a bit different than the SM213 one but this is for readability, and 
 * it's just nicer. I understand that the SM213 code isn't exactly like 
 * this. Please don't take off marks for that.
 */
#include <stdio.h>

/* I call the mysterious function with a switch in A5-c.s foo. If arg0
 * is outside the range [10..18] none of the code will execute and the
 * return value is 0. This isn't exactly like the SM213 code but it's
 * nicer in C. This way, if arg0 is outside the switch's range or is
 * an odd number within the range, foo returns 0, as it should. Else,
 * the cases are fairly simple and don't require special explanation.
 */
int
foo (int arg0, int arg1, int arg2)
{
    /* When arg0 is outside switch's range or is an odd int, no code
     * within switch will be executed. In all those cases, we simply
     * return 0.  */
    int return_val = 0;

    /* For each int in the switch's range we do some small
     * computation and store that as the return value.  */
    switch (arg0) {
        case 10:
            return_val = arg1 + arg2;
            break;
        case 12:
            return_val = arg1 - arg2;
            break;
        case 14:
            if (arg1 - arg2 > 0)
                return_val = 1;
            break;
        case 16:
            if (arg2 - arg1 > 0)
                return_val = 1;
            break;
        case 18:
            if (arg1 - arg2 == 0)
                return_val = 1;
            break;
    }

    return return_val;
}

/* The main function simply calls foo and prints the result. The
 * arguments can be set manually for testing. This isn't exactly how
 * the SM213 code works, clearly, but this is easier to debug and use.
 */
int
main (void)
{
    printf ("%d\n", foo (0, 0, 0));
    return 0;
}     

