#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"

#define NUM_READER_THREADS  4
#define NUM_WRITER_THREADS  1
#define NUM_PROCESSORS      2

uthread_monitor_t* mon;
int                c0=0, c1=0;

void do_some_work () {
  volatile int i;
  for (i=0; i<100; i++) {}
}

void read() {
  if (c0 != c1)
    printf ("reader error\n");
  do_some_work();
}

void write() {
  c0 = c0 + 1;
  c1 = c1 + 1;
  if (c0 != c1)
    printf ("writer error\n");
  do_some_work();
}

void read_ns() {
  read();
}

void write_ns() {
  write();
}

void read_mon() {
  uthread_monitor_enter (mon);
  read();
  uthread_monitor_exit (mon);
}

void write_mon() {
  uthread_monitor_enter (mon);
  write();
  uthread_monitor_exit (mon);
}

void read_rw_mon() {
  uthread_monitor_enter_read_only (mon);
  read();
  uthread_monitor_exit (mon);
}

struct drive_test_arg {
  int write;
  int count;
};

void (*read_proc)();
void (*write_proc)();

void* drive_test (void* arg_v) {
  struct drive_test_arg* dta = (struct drive_test_arg*) arg_v;
  int                    i;
  for (i=0; i<dta->count; i++) {
    if (dta->write)
      write_proc();
    else
      read_proc();
  }
}

void do_test (int count) {
  int                            i;
  uthread_t*                     t   [NUM_READER_THREADS+NUM_WRITER_THREADS];
  volatile struct drive_test_arg dta [NUM_READER_THREADS+NUM_WRITER_THREADS];

  mon = uthread_monitor_create();

  for (i=0; i<NUM_READER_THREADS+NUM_WRITER_THREADS; i++) {
    dta[i].count = count;
    dta[i].write = (i>=NUM_READER_THREADS);
    t[i] = uthread_create (drive_test, (void*) &dta[i]);
  }
  for (i=0; i<NUM_READER_THREADS+NUM_WRITER_THREADS; i++)
    uthread_join (t[i]);
  if (c0!=c1 || c0!=NUM_WRITER_THREADS*count)
    printf ("end error %d %d %d\n", c0, c1, NUM_WRITER_THREADS*count);
}

int main (int argc, char** argv) {
  char*                 usage = "usage: reader_writer_test t (0=ns,1=m,2=rwm) count";
  int                   i, type, count;
    
  if (argc==3) {
    type  = atoi (argv[1]);
    count = atoi (argv[2]);
  } else {
    printf ("%s\n", usage);
    exit   (-1);
  }
  
  switch (type) {
    case 0:
      read_proc  = read_ns;
      write_proc = write_ns;
      break;
    case 1:
      read_proc  = read_mon;
      write_proc = write_mon;
      break;
    case 2:
      read_proc  = read_rw_mon;
      write_proc = write_mon;
      break;
    default:
      printf ("%s\n", usage);
      exit   (-1);
  }
  uthread_init (NUM_PROCESSORS);
  do_test (count);
}