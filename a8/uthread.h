//
// Written by Mike Feeley, University of BC, 2010
// Do not redistribute or otherwise make available any portion of this code to anyone without written permission of the author.
//

struct uthread_TCB;
typedef struct uthread_TCB uthread_t;

void       uthread_init   (int num_processors);
uthread_t* uthread_create (void* (*star_proc)(void*), void* start_arg);
void       uthread_yield  ();
void*      uthread_join   (uthread_t* thread);
void       uthread_detach (uthread_t* thread);
uthread_t* uthread_self   ();

typedef volatile int spinlock_t;
void       spinlock_create (spinlock_t* lock);
void       spinlock_lock   (spinlock_t* lock);
void       spinlock_unlock (spinlock_t* lock);

