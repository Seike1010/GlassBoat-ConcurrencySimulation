# GlassBoat-ConcurrencySimulation

A multithreaded C program simulating tourist queue management for a
single-capacity glass boat attraction, demonstrating semaphore and mutex
usage for thread synchronization. Built as a group assignment for an
Operating Systems course.

## Overview

Three tourist threads arrive at a reception desk at random times, sign a
shared logbook to receive a ticket, then queue for a glass boat that can
only hold one rider at a time. The simulation shows how multiple threads
safely share limited resources (the boat) and shared data (the logbook)
without race conditions.

## Concurrency Concepts Demonstrated

- **Semaphore (`sem_t boat_sem`)** — models the single-capacity boat.
  Initialized to 0 (occupied by an "early bird" tourist) so that other
  threads correctly block until the boat becomes available
- **Mutex (`pthread_mutex_t logbook_mutex`)** — protects the shared
  ticket counter and logbook array from concurrent writes when multiple
  tourists sign in at the same time
- **Randomized arrival times** — each tourist thread sleeps for a random
  duration before "arriving," breaking the deterministic order of thread
  creation and forcing genuine concurrent access to shared resources
- **Producer/consumer-style handoff** — the boat is "returned" via
  `sem_post` after a simulated ride and cleaning period, waking the next
  waiting thread

## How It Works

1. Three tourist threads are created, each simulating a random walk time
   to the reception
2. Each thread locks the logbook mutex, takes a ticket number, records
   its name, then unlocks
3. Each thread waits on the boat semaphore until it's their turn
4. Whoever acquires the semaphore "rides" the boat, then releases it
   after a simulated cleaning delay
5. The main thread joins all tourist threads and prints the final
   boarding log in ticket order

## Tech Stack

C, POSIX threads (pthreads), semaphores, mutexes

## How to Run

```bash
gcc glassboat.c -o glassboat -lpthread
./glassboat
```

## Team

Group assignment — Operating Systems coursework, completed with Tung Xin
Yue and Diesca Jessica.
