#include <stdlib.h>
#include <stdio.h>

struct A {
  void (*ping) ();
  void (*pong) ();
};

void A_ping () { printf ("A_ping\n"); }
void A_pong () { printf ("A_pong\n"); }

struct A* new_A () {
  struct A* a = (struct A*) malloc (sizeof (struct A));
  a->ping = A_ping;
  a->pong = A_pong;
  return a;
}

struct B {
  void (*ping)();
  void (*pong)();
  void (*wiff)();
};

void B_ping () { printf ("B_ping\n"); }
void B_wiff () { printf ("B_wiff\n"); }

struct B* new_B () {
  struct B* b = (struct B*) malloc (sizeof (struct B));
  b->ping = B_ping;
  b->pong = A_pong;
  b->wiff = B_wiff;
  return b;
}

void foo (struct A* a) {
  a->ping ();
  a->pong ();
}

void bar () {
  foo (new_A ());
  foo ((struct A*) new_B ());
}

int main (char** argc, int argv) {
  bar ();
}