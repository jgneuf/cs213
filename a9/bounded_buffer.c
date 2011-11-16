//
// Written by Mike Feeley, University of BC, 2010
// Do not redistribute or otherwise make available any portion of this code to anyone without written permission of the author.
//


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "uthread.h"

#ifndef NUM_PROCESSORS
#define NUM_PROCESSORS 1
#endif
#ifndef NUM_THREAD_PAIRS
#define NUM_THREAD_PAIRS 2
#endif
#ifndef QUEUE_SIZE
#define QUEUE_SIZE 16
#endif
#ifndef NUM_ITERATIONS
#define NUM_ITERATIONS (QUEUE_SIZE*4)
#endif
#ifndef PRINT_EACH_THREAD_DONE
#define PRINT_EACH_THREAD_DONE 1
#endif
#ifndef PRINT_EACH_ITERATION
#define PRINT_EACH_ITERATION 1
#endif


/**
 * Bounded Buffer
 */

int queue[QUEUE_SIZE], queue_head=0, queue_tail=0;
uthread_monitor_t* queue_monitor;
uthread_cv_t*      queue_not_empty;
uthread_cv_t*      queue_not_full;


/**
 * initqueue 
 */

void initqueue () {
  queue_monitor   = uthread_monitor_create ();
  queue_not_empty = uthread_cv_create      (queue_monitor);
  queue_not_full  = uthread_cv_create      (queue_monitor);
}


/**
 * enqueue
 */

void enqueue (int val) {
  int i;
  if (((queue_tail+1) % QUEUE_SIZE)==queue_head)
    return; // the queue is full; there is no room for new element
  queue_tail = (queue_tail+1) % QUEUE_SIZE;
  queue[queue_tail] = val;
  queue_tail        = (queue_tail + 1) % QUEUE_SIZE;
}


/**
 * dequeue
 */

int dequeue () {
  int i;
  int val;
  
  if (queue_head==queue_tail)
    return -1; // the queue is empty; nothing to return;
  val        = queue[queue_head];
  queue_head = (queue_head + 1) % QUEUE_SIZE;
  
  return val;
}


/**
 * producer
 */

void* producer (void* id) {
  int i;
  for (i=0; i<NUM_ITERATIONS; i++)
    enqueue (((uintptr_t) id)*1000 + i);
#if PRINT_EACH_THREAD_DONE
  printf ("p %ld done\n",(uintptr_t) id); fflush (stdout);
#endif
}


/**
 * consumer
 */

void* consumer (void* id) {
  int i;
  for (i=0; i<NUM_ITERATIONS; i++)
#if PRINT_EACH_ITERATION
    printf ("%d consumed by %ld(%d)\n",dequeue(),(uintptr_t) id, i);
#else
    dequeue();
#endif
#if PRINT_EACH_THREAD_DONE
  printf ("c %ld done\n",(uintptr_t) id); fflush (stdout);
#endif
}


/**
 * main
 */

int main (int* argc, char** argv) {
  uintptr_t i;
  uthread_t *pt[NUM_THREAD_PAIRS], *ct[NUM_THREAD_PAIRS];
  uthread_init (NUM_PROCESSORS);
  initqueue ();
  for (i=0; i<NUM_THREAD_PAIRS; i++) {
    pt[i] = uthread_create (producer,(void*) i);
    ct[i] = uthread_create (consumer,(void*) i);
  }
  for (i=0; i<NUM_THREAD_PAIRS; i++) {
    uthread_join (pt[i]);
    uthread_join (ct[i]);
  }
}