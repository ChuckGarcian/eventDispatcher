
#include "event.h"

#ifndef QUEUE_H
#define QUEUE_H

/*Takes in an initial capacity
  Returns a pointer to an initlized kpQueue
*/
kpQueue * initQueue(int cap);

/*Takes a kpQueue and event struct
  The event is enqued
*/
void enqueue(kpQueue * kpq, event val);

event dequeue(kpQueue * kpq);
void destroyQueue(kpQueue * kpq);

/*Debug function for printing; Prints to queueDebugLog.txt*/
void print(kpQueue *kpq);
#endif // QUEUE_H