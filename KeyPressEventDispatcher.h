
#include "event.h"
#ifndef KPD_H
#define KPD_H
/* Initlizes the event dispatcher*/
void initDispatcher(char terminatingCharacter);

/*'listens'/reads keypresses that are written to stdin
   enqueues these presses as an event
*/
void * enqueueEvents(void * arg);

/*returns the next event*/
int pollEvent(event* event);

/*Terminates the dispatcher*/
void terminateDispatcher();

#endif // KPD_H