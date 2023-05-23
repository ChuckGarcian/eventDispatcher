#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

//#define DEBUG
#ifdef DEBUG
FILE *f;
#endif

/*Takes in a kpQueue and resizes it*/
void Qresize(kpQueue * kpq);

/*Takes in an initial capacity
  Returns a pointer to an initlized kpQueue
*/
kpQueue *initQueue(int cap) {
  kpQueue *res = malloc(sizeof(kpQueue));

  res->con = malloc(sizeof(event) * cap); // Allocate room for the internal container
  res->capacity = cap;
  res->indexOfFirst = 0;
  res->indexOfLast = -1;
  res->size = 0;
  
  #ifdef DEBUG
  f = fopen("queDebugLog.txt", "w");
  setbuf(f, NULL);
  print(res);
  #endif 

  return res;
}

/*Frees and destroys queue*/
void destroyQueue(kpQueue * kpq) {
    free(kpq->con);
    free(kpq);
}



/*Takes a kpQueue and event struct
  The event is enqued
*/
void enqueue(kpQueue *kpq, event val) {
  // not enough capacity so we resize
  if (kpq->size == kpq->capacity) {
    Qresize(kpq);
  }

  #ifdef DEBUG
  fprintf(f, "\n \nEnque Before: ");
  print(kpq);
  #endif

  // once capacity is available; mod makes it wrap arounds
  kpq->indexOfLast = (kpq->indexOfLast + 1) % kpq->capacity;
  kpq->size++;
  kpq->con[kpq->indexOfLast] = val;

  #ifdef DEBUG
  fprintf(f, "\n Enque After: ");
  print(kpq);
  #endif  
}

event dequeue(kpQueue * kpq) {
    #ifdef DEBUG
    fprintf(f, "\n \n Denque Before: ");
    print(kpq);
    #endif

    kpq->size--;
    event res = kpq->con[kpq->indexOfFirst];
    kpq->indexOfFirst = (kpq->indexOfFirst + 1) % kpq->capacity;

    #ifdef DEBUG
    fprintf(f, "\nDenque After: ");
    print(kpq);
    #endif

    return res;
}

void Qresize(kpQueue *kpq) {
    // Create new container with apprioiprate capacity
    event * newCon = malloc(sizeof(event) * kpq->capacity * 2);
    int oldConIndex = kpq->indexOfFirst;

    // Copy over from old container to new
    for (int i = 0; i < kpq->capacity; i++) {
        newCon[i] = kpq->con[oldConIndex];
        oldConIndex = (oldConIndex + 1) % kpq->capacity;
    }

    kpq->indexOfFirst = 0;
    kpq->indexOfLast = kpq->size - 1;

    free(kpq->con);
    kpq->con = newCon;
}

#ifdef DEBUG
/*Debug function for printing; Prints to queueDebugLog.txt*/
void print(kpQueue *kpq) {
  int size = kpq->size;

  int index = kpq->indexOfFirst; // Use to iterate through the queue without modifying indexFirst

  event curEvent;

  fprintf(f, "Queue ");
  fprintf(f, "Stats-> cap: %d, idxLast: %d, idxFirst: %d, size: %d \n", kpq->capacity, kpq->indexOfLast, kpq->indexOfFirst,  kpq->size);
  
  for (int i = 0; i < size; i++) {   
    curEvent = kpq->con[index];
    index = (index + 1) % kpq->capacity;
    fprintf(f, "EType: %d, KP: %c \n", curEvent.eventType, curEvent.kp);
  }
}
#endif