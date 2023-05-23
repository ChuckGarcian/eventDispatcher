
#ifndef EVENT_H
#define EVENT_H
// Possible Keypress events
enum eventType{
    UP_ARROW,
    DOWN_ARROW,
    RIGHT_ARROW,
    LEFT_ARROW,
    BACKSPACE,
    DELETE,
    QUIT_SEQUENCE,
    NON_CONTROL,
};

// Contains eventype and the key pressed if any
typedef struct event {
    enum eventType eventType;
    char kp; // contains Non control char pressed; NULL otherwise
} event;


 /*
   We maintain the status of the queue to prevent any potential
   race conditions; for example, Main thread tries to dequeue 
   while worker thread is in the middle of enqueing
   Generally an enqueue may not be preformed while a dequeue is in flight
   and vice versa
  */
enum enque_status {EVALID, EINVALID}; // enqueue status
enum deque_status {DVALID, DINVALID}; // deueue status

typedef struct eventQueue {
  event *con;   // internal container for the queue
  int capacity; // capacity of the internal container
  int indexOfLast; // index of the last element put in the queue; Last to come out
  int indexOfFirst; // index of the first element in the queue; first to come out
  int size; // size of the queue
  enum enque_status en_stat;
  enum deque_status de_stat;
 
} kpQueue;

#endif // EVENT_H