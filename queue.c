#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


FILE * f;
/*Takes in an initial capacity
  Returns a pointer to an initlized kpQueue
*/
kpQueue *initQueue(int cap) {
  kpQueue *res = malloc(sizeof(kpQueue));

  res->con = malloc(sizeof(event) * cap); // Allocate room for the internal container
  res->capacity = cap;
  res->indexOfFirst = 0;
  res->indexOfLast = 0;
  res->size = 0;

  event v = {v.eventType = 1, v.kp = 'l'};



  f = fopen("queDebugLog.txt", "w");
  setbuf(f, NULL);
  
  enqueue(res, v);
  dequeue(res);

  print(res);

  return res;
}

void destroyQueue(kpQueue * kpq) {
    free(kpq->con);
    free(kpq);
}

/*Takes a kpQueue and event struct
  The event is enqued
*/
void enqueue(kpQueue *kpq, event val) {
  if (kpq->size == kpq->capacity) {
    resize(kpq);
  }

  fprintf(f, "\n \nEnque Before: ");
  print(kpq);

  // once capacity is available
  kpq->indexOfLast = (kpq->indexOfLast + 1) % kpq->capacity;
  kpq->size++;
  kpq->con[kpq->indexOfLast] = val;

  fprintf(f, "\n Enque After: ");
  print(kpq);  
}

event dequeue(kpQueue * kpq) {
    fprintf(f, "\n \n Denque Before: ");
    print(kpq);

    kpq->size--;
    event res = kpq->con[kpq->indexOfFirst];
    kpq->indexOfFirst = (kpq->indexOfFirst + 1) % kpq->capacity;

    fprintf(f, "\nDenque After: ");
    print(kpq);

    return res;
}

void resize(kpQueue *kpq) {
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