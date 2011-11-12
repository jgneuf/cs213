Student Name: Jonathan Neufeld
Student ID: 30671093
CS ID: p9d8

3) Here are the times taken for readCount = 1000.
-s: 10.13s 
-t: 00.19s
-e: 00.48s (threadCount = 1000)
-e: 00.15s (threadCount = 150)

A threadCount of 150 seems to be optimal, yielding 0.146s. Each varient
was run five times and the minimum time was selected. threadCounts in
increments of 50 were tested to determine the optimal count.

For threadCounts of 50 and 100 the times steadily decreased, while 
anything above 150 saw time increases. 

The sequential version calls for a disk read then waits for the operation
to finish. Since the disk takes 10ms, the total time for 1000 disk reads
is relatively long -- over 10 seconds. Threaded and Executor methods run
must faster as they schedule a disk read then move on to schedule the next
disk read before the first completes. 

Executor with 1000 threads runs noticably slower than the threaded version 
and I'm unsure of why exactly this is. Perhaps it has to do with high 
overhead; I'm handing off the fine-grain control of the threads to some 
Executor with little idea of what exactly it does. My guess is there's a 
lot of work going on behind the scenes that slows it down, whereas the 
Threaded version is very simple and small, making it much quicker. With a
threadCount of 150 there's less overhead for Executor. Executor might
also have a more sophisticated scheduling system. We shouldn't need exactly
1000 threads, since some of the threads will complete before all disk reads
are finished. 150 could be all that run at a single time without any other
threads waiting idly.

Here are the times taken for large readCounts:

readCount		-t		-e		threadCount
10 000			0.65	0.45	500
100 000			4.74	1.53	1000
1 000 000		45.7	7.25	2500

The process for determining the best threadCount for Executor was the same
as before. The assignment PDF says Threaded should work better than
Executor up to a certain readCount, but I haven't found that to be the
case; if a good threadCount is chosen it's faster. (threadCounts of 400 to
800 actually worked just as well for readCount of 10 000, but 500 seemed
to produce the lowest results on average.)

The Threaded version is creating one thread for each read, which is far
too many. The memory and overhead going into that is probably what slows
it down so much, maybe even exponential slowdown. The Executor is probably
making some smart scheduling decisions behind the scenes, and a smaller
threadCount is better since it's using far less memory; there's much less
overhead.

4) 
(6)/(7) Functions involved in creating and beginning a thread:

> uthread_create
> > uthread_new_thread 
> > > uthread_alloc
> > ready_queue_enqueue
> > > spinlock_lock
> > > enqueue
> > > spinlock_unlock

uthread_create makes calls to uthread_new_thread and ready_queue_enqueue.
It takes a function pointer and an argument, which it uses to set up the
new thread. uthread_new_thread allocates a TCB for the new thread, using the
function pointer, and sets its initial state to nascent. This new thread is
then pushed onto the ready queue by ready_queue_enqueue. It first places a
lock around the thread, then enqueues it, finally releasing the lock.

In the ping_pong example, each thread started by calling uthread_join. The
join function places a lock on the thread its given and, if its state is not
dead, stops the thread. uthread_stop dequeues a thread from the ready queue
and calls uthread_switch. The switch can spin on the dequeued thread if it's
running. Switch restores the stack of the to_thread from its TCB. It checks
if the from_thread is dying, i.e. it finished its execution. If it has, switch
will free the thread. Otherwise if the state is nascent, set state to running
and call the function. Otherwise the state is simply running.

There's no pre-emption in any of the function mentioned so far. That's why,
when ping_pong.c calls uthread_join on ping_thread, the call to ping from
switch runs to completion, i.e. prints 'I' 100 times. It then returns to the
main function in ping_pong.c, where the next call is to uthread_join on 
pong_thread. As before, this will end up calling switch, which will run pong
to completion, printing 'O' 100 times. 

(8) Execution remains largely the same, except after uthread_join is called
on ping_thread from main, ping will print a single 'I' then call uthread_yield.
This yield causes the currently running thread -- ping_thread -- to be enqueued 
via ready_queue_enqueue. uthread_yield then calls uthread_stop, passing the
new state for ping_thread as runnable. uthread_stop dequeues a thread from the
ready queue and calls switch, passing the dequeued thread as the thread to
switch to and the stopping thread (ping_thread) state (this time it's runnable).
uthread_switch will run as before, pushing from_thread's registers onto the
stack and popping to_thread's registers back. If from_thread has finished,
i.e. its dying, then it's freed. Initially, since pong_thread is set to nascent,
switch will set its state to running and will call pong. In the more general
case where both ping and pong have been called, we simply set to_thread's
state to running and continue -- since the stack now has to_thread's data,
we will automatically return to to_thread's execution.

(9) The output is seemingly random aside from invariant number of 'I's and 'O's,
i.e. the order they appear in is unpredicatble. Since each core can run a thread
at the same time, the order execution is erratic. It could be the case that one
processor is faster than the other, thus working through execution quicker and
printing several characters before the other prints a single one. It's also the
case that these processors are running numerous other threads aside from what
I'm viewing. 

NUM_ITERATIONS	1-core	2-core
1 000			0.05s	0.03s
10 000			0.47s	0.41s
100 000			4.71s	4.11s

I'm not sure how accurate this can be (or maybe the assignment PDF had a type?).
Yes, taking advantage of 2 cores speeds things up, but most of this time is
going into printing characters to the screen.

