/* There's no real function here, it's just one assembly program.
 * The array a is local, so no need to call malloc. This program
 * simply finds the largest element of an int array.  */
int a[5] = {2,4,16,7,1};
int max  = -1;
int i;

for (i = 0; i < 5; i++)
{
    if (a[i] > max)
        max = a[i];
}


