/*
    pull based Keypress event dispatcher; IE services keypress event polling when requested
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "KeyPressEventDispatcher.h"
#include "queue.h"
#include <pthread.h>

#define initialEvenCapacity 200
/* Initlizes the event dispatcher*/
void initDispatcher() {
    // event queue
    kpQueue * kpq = initQueue(initialEvenCapacity);
    pthrea_t pt; // event listener thread

    // passes to the thread create sys call
    void (*worker) ();
    worker = enqueueEvents; 

    pthread_create(&pt, NULL, worker, NULL);
        
}

/*'listens'/reads keypresses that are written to stdin
   enqueues these presses as an event
*/
void * enqueueEvents(void * arg) {

    printf("This is working!");
}

/*returns the next event*/
int pollEvent(event * event);

/*Terminates the dispatcher*/
void terminateDispatcher();