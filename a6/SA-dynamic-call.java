class A {
  void ping () { System.out.printf ("A.ping\n"); }
  void pong () { System.out.printf ("A.pong\n"); }
}

class B extends A {
  void ping () { System.out.printf ("B.ping\n"); }
  void wiff () { System.out.printf ("B.wiff\n"); }
}

static void foo (A a) {
  a.ping ();
  a.pong ();
}

static void bar () {
  foo (new A());
  foo (new B());
}
