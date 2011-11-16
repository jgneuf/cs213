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

struct uthread_semaphore;
typedef struct uthread_semaphore uthread_semaphore_t;

uthread_semaphore_t*          uthread_semaphore_create (int initial_value);
void                          uthread_semaphore_free   (uthread_semaphore_t* sem);
void                          uthread_V                (uthread_semaphore_t* sem);
void                          uthread_P                (uthread_semaphore_t* sem);

struct uthread_monitor;
typedef struct uthread_monitor uthread_monitor_t;
struct uthread_cv;
typedef struct uthread_cv uthread_cv_t;

uthread_monitor_t* uthread_monitor_create          ();
void               uthread_monitor_enter           (uthread_monitor_t* monitor);
void               uthread_monitor_enter_read_only (uthread_monitor_t* monitor);
void               uthread_monitor_exit            (uthread_monitor_t* monitor);
uthread_cv_t*      uthread_cv_create               ();
void               uthread_cv_wait                 (uthread_cv_t* cv);
void               uthread_cv_notify               (uthread_cv_t* cv);
void               uthread_cv_notify_all           (uthread_cv_t* cv);
