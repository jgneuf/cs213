/* The global array arr is accessed by function foo several times. Each
 * time, an integer pointer a is created locally (on the stack) to point
 * to the global array. I'm just reverse engineering the SM213 code, but
 * it looks like there's more than one way to do this. I could forget the
 * local array a and simply use *(arr + arg1) = *(arr + arg1) + arg0, for
 * example. Or I could call foo using foo (3, 4). But the code below
 * looks more like a direct translation of the SM213 code. At least, it's
 * pretty close. I don't think I have to manually set each element of arr
 * to 0 since it's a global array, but that is something I considered
 * doing.  */

/* Global int array of 10 elements.  */
int *arr = (int *) malloc (sizeof (int) * 10);

/* Set arr[arg1] to a[arg1] + arg0 and return the result. I return it
 * because r0 is usually the return value in SM213.  */
int
foo (int arg0, int arg1)
{
    int *a = arr;
    return a[arg1] = a[arg1] + arg0;
}

/* Make two calls to foo using local variables as arguments.  */
void
bar ()
{
    int a = 3, b = 4, c = 1, d = 2;
    foo (a, b);
    foo (c, d);
}

