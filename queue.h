

typedef struct queue {
  event *con;   // internal container for the queue
  int capacity; // capacity of the internal container
  int indexOfLast; // index of the last element put in the queue; Last to come out
  int indexOfFirst; // index of the first element in the queue; first to come out
  int size; // size of the queue
} kpQueue;

/*Takes in an initial capacity
  Returns a pointer to an initlized kpQueue
*/
kpQueue * initQueue(int cap);

/*Takes a kpQueue and event struct
  The event is enqued
*/
void enqueue(kpQueue * kpq, event val);

event dequeue(kpQueue * kpq);
/*Takes in a kpQueue and resizes it*/
void resize(kpQueue * kpq);
