#if __CYGWIN__
#ifndef NATIVE_STACK_SIZE
#define NATIVE_STACK_SIZE (1*1024*1024)
#endif
#ifndef PTHREAD_SUPPORT
#define PTHREAD_SUPPORT 1
#endif
#ifndef PTHREAD_SETSTACK_SUPPORT
#define PTHREAD_SETSTACK_SUPPORT 0
#endif
#else
#ifndef NATIVE_STACK_SIZE
#define NATIVE_STACK_SIZE (8*1024*1024)
#endif
#ifndef PTHREAD_SUPPORT
#define PTHREAD_SUPPORT  1
#endif
#ifndef PTHREAD_SETSTACK_SUPPORT
#define PTHREAD_SETSTACK_SUPPORT 0
#endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#if PTHREAD_SUPPORT
#include <pthread.h>
#endif
#include "uthread.h"

#define TS_NASCENT 0
#define TS_RUNNING 1
#define TS_RUNABLE 8
#define TS_BLOCKED 3
#define TS_DYING   4
#define TS_DEAD    5

#define STACK_SIZE     (8*1024*1024)

//
// SPINLOCKS
//

/**
 * spinlock_create
 */

void spinlock_create (spinlock_t* lock) {
	/* Set value of lock to 0, indicating it's free. */
	*lock = 0;
}

/**
 * spinlock_lock
 */

void spinlock_lock (spinlock_t* lock) {
	int already_held=1;

	/* Spin on lock until it's free, then lock it. */
	do {
		/* While this lock is held, spin. */
		while (*lock);

		/* Lock is possibly free, attempt to lock the shared variable. */
		asm volatile ("xchg  %0, %1\n\t" : "=m" (*lock), "=r" (already_held) : "1" (already_held));
	} while (already_held);
}

/**
 * spinlock_unlock
 */

void spinlock_unlock (spinlock_t* lock) {
	/* Free the lock. */
	*lock = 0;
}

//
// THREAD CONTROL BLOCK
//

struct uthread_TCB {
  volatile int         state;                 
  volatile uintptr_t   saved_sp;              
  void*              (*start_proc) (void*);
  void*                start_arg;
  void*                return_val;
  void*                stack;
  spinlock_t           join_spinlock;
  uthread_t*           joiner;
  struct uthread_TCB*  next;
};

//
// THREAD QUEUE
//

struct uthread_queue {
  uthread_t  *head, *tail;
};

typedef struct uthread_queue uthread_queue_t;

/**
 * initqueue
 */

static void initqueue (uthread_queue_t* queue) {
  queue->head = 0;
  queue->tail = 0;
}

/**
 * enqueue
 */

static void enqueue (uthread_queue_t* queue, uthread_t* thread) {
  thread->next = 0;
  if (queue->tail)
    queue->tail->next = thread;
  queue->tail = thread;
  if (queue->head==0)
    queue->head = queue->tail;
}

/**
 * dequeue
 */

static uthread_t* dequeue (uthread_queue_t* queue) {
  uthread_t* thread;
  if (queue->head) {    
    thread = queue->head;
    queue->head = queue->head->next;
    if (queue->head==0)
      queue->tail=0;
  } else 
    thread=0;
  return thread;
}

//
// READY QUEUE
//

static spinlock_t      ready_queue_spinlock;
static uthread_queue_t ready_queue;

/**
 * ready_queue_enqueue
 */

static void ready_queue_enqueue (uthread_t* thread) {
  spinlock_lock (&ready_queue_spinlock);
  enqueue (&ready_queue, thread);
  spinlock_unlock (&ready_queue_spinlock);
}

/**
 * ready_queue_dequeue
 */

static uthread_t* ready_queue_dequeue () {
  uthread_t* thread;
  spinlock_lock (&ready_queue_spinlock);
  thread = dequeue (&ready_queue);
  spinlock_unlock (&ready_queue_spinlock);
  return thread;
}

static void ready_queue_init () {
  spinlock_create (&ready_queue_spinlock);
  initqueue       (&ready_queue);
}


//
// INITIALIZATION 
//

static uthread_t* uthread_alloc      ();
static uthread_t* uthread_new_thread (void* (*)(void*), void*);

static uthread_t* base_thread;
static uintptr_t  base_sp_lower_bound, base_sp_upper_bound;

#if PTHREAD_SETSTACK_SUPPORT==0
#define MAX_PTHREADS 100
static spinlock_t num_pthreads_spinlock = 0;
static int        num_pthreads = 0;
static uthread_t* pthread_base_thread[MAX_PTHREADS];
static uintptr_t  pthread_base_sp_lower_bound[MAX_PTHREADS], pthread_base_sp_upper_bound[MAX_PTHREADS];
#endif



static void* pthread_base (void* arg) {
#if PTHREAD_SETSTACK_SUPPORT==0
  if (arg) {
    spinlock_lock (&num_pthreads_spinlock);
    assert (num_pthreads < MAX_PTHREADS);
    pthread_base_thread         [num_pthreads] = (uthread_t*) arg;
    pthread_base_sp_upper_bound [num_pthreads] = (((uintptr_t)&arg) + 1024);
    pthread_base_sp_lower_bound [num_pthreads] = (((uintptr_t)&arg) - NATIVE_STACK_SIZE);
    num_pthreads += 1;
    spinlock_unlock (&num_pthreads_spinlock);
  }
#endif
  while (1)
    uthread_yield ();  // XXX block pthread
}

/**
 * uthread_init
 */

void uthread_init (int num_processors) {
  int dummy_local, i;
  uthread_t* uthread;
#if PTHREAD_SUPPORT
  pthread_t* pthread;
  pthread_attr_t attr;
#else
  assert (num_processors==1);
#endif
  
  base_sp_upper_bound = (((uintptr_t)&dummy_local) + 1024);
  base_sp_lower_bound = (((uintptr_t)&dummy_local) - NATIVE_STACK_SIZE);
  base_thread         = uthread_alloc ();
  base_thread->state  = TS_RUNNING;
  base_thread->stack  = 0;
  ready_queue_init      ();
  uthread_create        (pthread_base, 0);
#if PTHREAD_SUPPORT
  for (i=0; i<num_processors-1; i++) {
    uthread = uthread_new_thread (pthread_base, 0);
    uthread->state = TS_RUNNING;
    pthread = (pthread_t*) malloc (sizeof (pthread_t));
    pthread_attr_init (&attr);
#if PTHREAD_SETSTACK_SUPPORT
    pthread_attr_setstack (&attr, (void*) ((((uintptr_t) uthread->stack) + STACK_SIZE - 1) & ~(STACK_SIZE - 1)), STACK_SIZE);
#endif
    pthread_create (pthread, &attr, pthread_base, uthread);
  }
#endif
}

//
// UTHREAD PRIVATE IMPLEMENTATION 
//

static void       uthread_stop  (int);
static void       uthread_start (uthread_t*);
static void       uthread_free  (uthread_t*);

/**
 * uthread_alloc
 */

static uthread_t* uthread_alloc () {
  uthread_t* thread = (uthread_t*) malloc (sizeof (uthread_t));
  thread->state      = TS_NASCENT;
  thread->start_proc = 0;
  thread->start_arg  = 0;
  thread->stack      = 0;
  thread->saved_sp   = 0;
  thread->joiner     = 0;
  spinlock_create (&thread->join_spinlock);
  return thread;
}

/**
 * uthread_new_thread
 */

static uthread_t* uthread_new_thread (void* (*start_proc)(void*), void* start_arg) {
  uthread_t* thread  = uthread_alloc ();
  thread->state      = TS_NASCENT;
  thread->start_proc = start_proc;
  thread->start_arg  = start_arg;
  thread->stack      = malloc (STACK_SIZE * 2);
  thread->saved_sp   = ((((uintptr_t) thread->stack) + STACK_SIZE - 1) & ~(STACK_SIZE - 1)) + STACK_SIZE;
  *(uthread_t**) (thread->saved_sp -1 & ~(STACK_SIZE -1)) = thread;
  asm volatile (
#if __LP64__
// IA32-64
#define CLOBBERED_REGISTERS "%rax", "%rcx"
                "movq  %%rsp, %%rax\n\t"
                "movq %c1(%0), %%rsp\n\t"
                "subq $512, %%rsp\n\t"    // frame for uthread_switch
                "pushq $0\n\t"
                "pushq $0\n\t"
                "pushq $0\n\t"
                "pushq $0\n\t"
                "pushq $0\n\t"
                "pushq %%r8\n\t"
                "pushq %%r9\n\t"
                "pushq %%r10\n\t"
                "pushq %%r11\n\t"
                "pushq %%r12\n\t"
                "pushq %%r13\n\t"
                "pushq %%r14\n\t"
                "pushq %%r15\n\t"
                "pushfq\n\t"
                "movq  %%rsp, %%rcx\n\t"
                "addq  $256, %%rcx\n\t" // locate saved ebp within frame
                "pushq %%rcx\n\t"
                "movq %%rsp, %c1(%0)\n\t"
                "movq  %%rax, %%rsp\n\t"
#else
// IA32-32
#define CLOBBERED_REGISTERS "%eax", "%ecx"
                "movl  %%esp, %%eax\n\t"
                "movl  %c1(%0), %%esp\n\t"
                "subl  $512, %%esp\n\t"
                "pushl $0\n\t"
                "pushl $0\n\t"
                "pushl $0\n\t"
                "pushl $0\n\t"
                "pushl $0\n\t"
                "pushfl\n\t"
                "movl  %%esp, %%ecx\n\t"
                "addl  $256, %%ecx\n\t"
                "pushl %%ecx\n\t"
                "movl %%esp, %c1(%0)\n\t"
                "movl  %%eax, %%esp\n\t"
#endif
                : : "r" (thread), 
                "i" ((uintptr_t)&thread->saved_sp-(uintptr_t)thread) 
                : CLOBBERED_REGISTERS);
  return thread;
}

/**
 * uthread_switch
 */

static void uthread_switch (uthread_t* to_thread, int from_thread_state) {
  uthread_t* from_thread = uthread_self ();

  if (from_thread == to_thread)
   return;
  
  // May be racing with blocking of to_thread so spin until it has blocked
  while (to_thread->state == TS_RUNNING) {}
  
  asm volatile (
#if __LP64__
// IA32-64
                "pushq %%rbx\n\t"
                "pushq %%rcx\n\t"
                "pushq %%rdx\n\t"
                "pushq %%rsi\n\t"
                "pushq %%rdi\n\t"
                "pushq %%r8\n\t"
                "pushq %%r9\n\t"
                "pushq %%r10\n\t"
                "pushq %%r11\n\t"
                "pushq %%r12\n\t"
                "pushq %%r13\n\t"
                "pushq %%r14\n\t"
                "pushq %%r15\n\t"
                "pushfq\n\t"
                "pushq %%rbp\n\t"
                "movq  %%rsp, %c5(%1)\n\t"
                "movl  %3, %c4(%1)\n\t"
                "movq  %c5(%2), %%rsp\n\t"
                "popq  %%rbp\n\t"
                "movq  %1, %0\n\t"
                "popfq\n\t"
                "popq  %%r15\n\t"
                "popq  %%r14\n\t"
                "popq  %%r13\n\t"            
                "popq  %%r12\n\t"
                "popq  %%r11\n\t"
                "popq  %%r10\n\t"
                "popq  %%r9\n\t"
                "popq  %%r8\n\t"
                "popq  %%rdi\n\t"
                "popq  %%rsi\n\t"
                "popq  %%rdx\n\t"
                "popq  %%rcx\n\t"
                "popq  %%rbx\n\t"
#else
// IA32-32
                "pushl %%ebx\n\t"
                "pushl %%ecx\n\t"
                "pushl %%edx\n\t"
                "pushl %%esi\n\t"
                "pushl %%edi\n\t"
                "pushfl\n\t"
                "pushl %%ebp\n\t"
                "movl  %%esp, %c5(%1)\n\t"
                "movl  %3, %c4(%1)\n\t"
                "movl  %c5(%2), %%esp\n\t"
                "popl  %%ebp\n\t"
                "movl  %1, %0\n\t"
                "popfl\n\t"
                "popl  %%edi\n\t"
                "popl  %%esi\n\t"
                "popl  %%edx\n\t"
                "popl  %%ecx\n\t"
                "popl  %%ebx\n\t"
#endif
                : "=m" (from_thread) : "r" (from_thread), "r" (to_thread), "r" (from_thread_state), 
                "i" ((uintptr_t)&to_thread->state-(uintptr_t)to_thread), 
                "i" ((uintptr_t)&to_thread->saved_sp-(uintptr_t)to_thread));
  
  if (from_thread->state == TS_DYING) {
    spinlock_lock (&from_thread->join_spinlock);
    if (from_thread->joiner == (uthread_t*) -1)
      uthread_free (from_thread);
    else {
      if (from_thread->joiner != 0)
        uthread_start (from_thread->joiner);
      from_thread->state = TS_DEAD;  
      spinlock_unlock (&from_thread->join_spinlock);
      // at this point uthread_detach could free from_thread, so don't touch it after setting it to DEAD
    }
  }
  
  to_thread = uthread_self ();
  if (to_thread->state == TS_NASCENT) {
    to_thread->state      = TS_RUNNING;
    to_thread->return_val = to_thread->start_proc (to_thread->start_arg);
    uthread_stop (TS_DYING);
  } else
    to_thread->state = TS_RUNNING;
}

/**
 * uthread_stop
 */

static void uthread_stop (int stopping_thread_state) {
  uthread_t* to_thread = ready_queue_dequeue ();
  assert         (to_thread);
  uthread_switch (to_thread, stopping_thread_state);
}

/**
 * uthread_start
 */

static void uthread_start (uthread_t* thread) {
  // XXX Possible race with blocking of this thread would result in the
  // thread on the ready queue in the TS_BLOCKED state
  if (thread->state != TS_RUNNING)
    thread->state = TS_RUNABLE;
  ready_queue_enqueue (thread);
}

/**
 * uthread_free
 */

static void uthread_free (uthread_t* thread) {
  if (thread->stack)
    free (thread->stack);
  free (thread);
}


//
// UTHREAD PUBLIC INTERFACE
//

/**
 * uthread_create
 */

uthread_t* uthread_create (void* (*start_proc)(void*), void* start_arg) {
  uthread_t* thread = uthread_new_thread  (start_proc, start_arg);
  ready_queue_enqueue (thread);
  return thread;
}                                   

/**
 * uthread_self
 */

uthread_t* uthread_self () {
  int dummy_local, i;

  if ((uintptr_t)&dummy_local >= base_sp_lower_bound && (uintptr_t) &dummy_local <= base_sp_upper_bound)
    return base_thread;
  else {
#if PTHREAD_SETSTACK_SUPPORT==0
    for (i=0; i<num_pthreads; i++)
      if ((uintptr_t)&dummy_local >= pthread_base_sp_lower_bound[i] && (uintptr_t) &dummy_local <= pthread_base_sp_upper_bound[i])
        return pthread_base_thread[i];
#endif
    return *(uthread_t**) (void*) (((uintptr_t) &dummy_local) & ~(STACK_SIZE-1));
  }
}

/**
 * uthead_yield
 */

void uthread_yield () {
  ready_queue_enqueue (uthread_self ());
  uthread_stop        (TS_RUNABLE); 
}

void* uthread_join (uthread_t* thread) {
  void* return_val;
  
  if (thread->joiner == 0) {
    spinlock_lock (&thread->join_spinlock);
    if (thread->state != TS_DEAD) {
      thread->joiner = uthread_self ();
      spinlock_unlock (&thread->join_spinlock);
      uthread_stop (TS_BLOCKED);
    }
    return_val = thread->return_val;
    uthread_free (thread);
  } else 
    return 0;
  
  return return_val;
}

/**
 * uthread_detach
 */

void uthread_detach (uthread_t* thread) {
  if (thread->joiner == 0) {
    spinlock_lock (&thread->join_spinlock);
    if (thread->state != TS_DEAD) {
      thread->joiner = (uthread_t*) -1;
      spinlock_unlock (&thread->join_spinlock);
    } else 
      uthread_free (thread);
  }
}

//
// MONITORS AND CONDITIONAL VARIABLES
//

// XXX multi-reader monitor: need reader count and need to re-start ALL blocked readers on EXIT

struct uthread_monitor {
  uthread_t*      holder;		// Thread using this lock
  spinlock_t      spinlock;		// Spinlock holding this monitor
  uthread_queue_t waiter_queue;	// Queue of threads waiting for this lock
};

struct uthread_cv {
  // TODO
};

/**
 * uthread_monitor_create
 */

uthread_monitor_t* uthread_monitor_create () {
	/* Allocate memory of monitor and set holder to null. */
	uthread_monitor_t* monitor = (uthread_monitor_t*) malloc (sizeof (uthread_monitor_t));
	monitor->holder = 0;

	/* Create spin lock for monitor (initially free). */
	spinlock_create (&monitor->spinlock);

	/* Create TCB queue for this monitor. */
	initqueue       (&monitor->waiter_queue);

	return monitor;
}

/**
 * uthread_monitor_free
 */

void uthread_monitor_free (uthread_monitor_t* monitor) {
	/* Deallocate given monitor. */
	free (monitor);
}

/**
 * uthread_monitor_enter
 */

void uthread_monitor_enter (uthread_monitor_t* monitor) {
	/* Lock the spinlock of this monitor. It will spin until we
	 * it's free, assuring us we can enter this critical section. */
	spinlock_lock (&monitor->spinlock);

	/* While another thread holds this monitor, enqueue current thread
	 * and release the spinlock on this monitor so the other thread can
	 * finish. When control returns, set the spinlock to held and try again. */
	while (monitor->holder) {
		enqueue         (&monitor->waiter_queue, uthread_self ());
		spinlock_unlock (&monitor->spinlock);
		uthread_stop    (TS_BLOCKED);
		spinlock_lock   (&monitor->spinlock);
	}

	/* Current thread now has monitor, set holder to current thread. Critical
	 * section of this code is over, so release spinlock. */
	monitor->holder = uthread_self ();
	spinlock_unlock (&monitor->spinlock);
}

/**
 * uthread_monitor_enter_read_only
 */

void uthread_monitor_enter_read_only (uthread_monitor_t* monitor) {
  // TODO
}



/**
 * uthread_monitor_exit
 */

void uthread_monitor_exit (uthread_monitor_t* monitor) {
	uthread_t* waiter_thread;

	/*  */
  
	spinlock_lock (&monitor->spinlock);
	assert (monitor->holder == uthread_self ());
	monitor->holder = 0;
	waiter_thread   = dequeue (&monitor->waiter_queue);
	spinlock_unlock (&monitor->spinlock);
	if (waiter_thread)
		uthread_start (waiter_thread);
}

uthread_cv_t* uthread_cv_create (uthread_monitor_t* monitor) {
  // TODO 
}

void uthread_cv_wait (uthread_cv_t* cv) {
  // TODO
}

void uthread_cv_notify (uthread_cv_t* cv) {
  // TODO
}

void uthread_cv_notify_all (uthread_cv_t* cv) {
  // TODO
}
