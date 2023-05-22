#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*Takes in an initial capacity
  Returns a pointer to an initlized kpQueue
*/
kpQueue *initQueue(int cap) {
  kpQueue *res = malloc(sizeof(kpQueue));

  res->con =
      malloc(sizeof(event) * cap); // Allocate room for the internal container
  res->capacity = cap;
  res->indexOfFirst = 0;
  res->indexOfLast = 0;
  res->size = 0;
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

  // once capacity is available
  kpq->indexOfLast = (kpq->indexOfLast + 1) % kpq->capacity;
  kpq->size++;
  kpq->con[kpq->indexOfLast] = val;
}

event dequeue(kpQueue * kpq) {
    kpq->size--;
    event res = kpq->con[kpq->indexOfFirst];
    kpq->indexOfFirst = (kpq->indexOfFirst + 1) % kpq->capacity;
    return res;
}

void resize(kpQueue *kpq) {
    // Create new container with apprioiprate capacity
    event * newCon = malloc(sizeof(event) * kpq->capacity * 2);
    
    int oldConIndex = kpq->indexOfFirst;

    // Copy over from old container to new
    for (int i = 0; i < kpq->size; i++) {
        newCon[i] = kpq->con[oldConIndex % kpq->capacity];
        oldConIndex++;
    }

    kpq->indexOfFirst = 0;
    kpq->indexOfLast = oldConIndex;

    free(kpq->con);
    kpq->con = newCon;
}


