#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* The code that caused the bugs in tests 2 and 4 was initially
 * commented out. By the time I got to test 6 I saw it was time
 * to make more significant changes to the stack implementation.
 * After the fix (described below), the buggy pointers in tests
 * 2 and 4 could run without dangling pointer errors.  */
 
/* Max characters in string.  */
#define STACK_NAME_SIZE 100

/* Element of stack has a string.  */
struct StackElement {
  char   name[STACK_NAME_SIZE];
  struct StackElement *next;
};

/* Track the top of the stack -- default NULL.  */
struct StackElement *stackTop = 0;

/* Push takes a pointer to a stack element. Functions that
 * use push MUST create the elements locally and pass
 * the address of the stack element to push. See usage
 * examples in the tests below.  */
void push (struct StackElement* e) {
  e->next  = stackTop;
  stackTop = e;
}

/* Pop returns a pointer to the element coming off the stack.
 * There is no need to free anything here. Assuming the calling
 * function is using locally created stack elements, they will
 * be destroyed at the end of the functions call. Regardless,
 * management of memory is external to the stack implementation.  */
struct StackElement* pop () {
  struct StackElement* e = stackTop;
  stackTop = e->next;
  return e;
}

/* In use, this is multiplied by sizeof(int), so it actually
 * represents the size of a struct StackElement.  */
#define INT_ARRAY_SIZE (sizeof(struct StackElement)/sizeof(int))

/* Test 1 should print "zero".  */
void test1 () {
  printf ("test1:\n");

  struct StackElement e;
  strncpy (e.name, "Zero", STACK_NAME_SIZE);
  push (&e);

  printf ("%s\n", pop ());
}

/* Test 2 should print "zero".  */
void test2 () {
  printf ("test2:\n");

  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);

  struct StackElement e;
  strncpy (e.name, "Zero", STACK_NAME_SIZE);
  push (&e);

  while (ipc - ip <= INT_ARRAY_SIZE) 
    *(ipc++) = 0;

  printf ("%s\n", pop ());
}

/* Test 3 should print "one" and "zero".  */
void test3 () {
  printf ("test3:\n");

  struct StackElement e1;
  strncpy (e1.name, "Zero", STACK_NAME_SIZE);
  push (&e1);

  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);

  struct StackElement e2;
  strncpy (e2.name, "One", STACK_NAME_SIZE);
  push (&e2);

  printf ("%s\n", pop ());
  printf ("%s\n", pop ());
}

/* Test 4 should print "one" and "zero".  */
void test4 () {
  printf ("test4:\n");

  struct StackElement e1;
  strncpy (e1.name, "Zero", STACK_NAME_SIZE);
  push (&e1);

  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);

  struct StackElement e2;
  strncpy (e2.name, "One", STACK_NAME_SIZE);
  push (&e2);

  while (ipc - ip <= INT_ARRAY_SIZE) 
    *(ipc++) = 0;

  printf ("%s\n", pop ());
  printf ("%s\n", pop ());
}

/* Test 5 should print "one" and "zero".  */
void test5 () {
  printf ("test5:\n");

  struct StackElement e1;
  strncpy (e1.name, "Zero", STACK_NAME_SIZE);
  push (&e1);

  struct StackElement e2;
  strncpy (e2.name, "One", STACK_NAME_SIZE);
  push (&e2);

  printf ("%s\n", pop ()); 
  printf ("%s\n", pop ());

  struct StackElement e3;
  strncpy (e3.name, "Two", STACK_NAME_SIZE);
  push (&e3);

  struct StackElement e4;
  strncpy (e4.name, "Three", STACK_NAME_SIZE);
  push (&e4);
}

/* Test 6 should print "one" and "zero".  */
void test6 () {
  printf ("test6:\n");

  char *x[2];

  struct StackElement e1;
  strncpy (e1.name, "Zero", STACK_NAME_SIZE);
  push (&e1);

  struct StackElement e2;
  strncpy (e2.name, "One", STACK_NAME_SIZE);
  push (&e2);

  x[0] = (pop ())->name;
  x[1] = (pop ())->name;

  struct StackElement e3;
  strncpy (e3.name, "Two", STACK_NAME_SIZE);
  push (&e3);

  struct StackElement e4;
  strncpy (e4.name, "Three", STACK_NAME_SIZE);
  push (&e4);

  printf ("%s\n", x[0]);
  printf ("%s\n", x[1]);
}

/* Tell the user they messed up.  */
#define USAGE "usage: dangling-pointer test-number"

/* Main: handle test cases from command line.  */
int main (int argc, char** argv) {
  if (argc != 2) {
    printf ("%s\n", USAGE);
    exit (EXIT_FAILURE);
  }
  switch (strtol (argv[1],0,10)) {
    case 1:
      test1 ();
      break;
    case 2:
      test2 ();
      break;
    case 3:
      test3 ();
      break;
    case 4:
      test4 ();
      break;
    case 5:
      test5 ();
      break;
    case 6:
      test6 ();
      break;
    default:
      printf ("%s (where test-number is 1-6)\n", USAGE);
      exit (EXIT_FAILURE);
  }
}