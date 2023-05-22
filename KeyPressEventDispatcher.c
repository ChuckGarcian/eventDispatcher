/*
    pull based Keypress event dispatcher; IE services keypress event polling when requested
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "KeyPressEventDispatcher.h"
#include "queue.h"

#include <pthread.h>

//Todo fix header file situation; event.h shoudlnt be its own thing
#define initialEvenCapacity 200
/* Initlizes the event dispatcher*/
void initDispatcher() {
    // event queue
    kpQueue * kpq = initQueue(initialEvenCapacity);
    pthread_t pt; // event listener thread

    // passes to the thread create sys call
   // void * (*worker) (void *);
    //worker = enqueueEvents; 

    pthread_create(&pt, NULL, enqueueEvents, NULL);

    pthread_join(pt, NULL);
        
}

/*'listens'/reads keypresses that are written to stdin
   enqueues these presses as an event
*/
void * enqueueEvents(void * arg) {
        
}

int pollEvent(event * event);

/*Terminates the dispatcher*/
void terminateDispatcher();
