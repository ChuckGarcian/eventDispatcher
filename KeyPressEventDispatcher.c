/*
    pull based Keypress event dispatcher; IE services keypress event polling
   when requested
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "KeyPressEventDispatcher.h"
#include "queue.h"

#include <pthread.h>
//#define DEBUG
#define initialEventCapacity 200

kpQueue *kpq;
char tc;                   // terminating character
struct termios newt, oldt; // to change/save line displcine
pthread_t pt;              // event listener thread
#ifdef DEBUG
FILE *file; // For debuging
#endif

enum eventType escapeChar();
void makeRaw();

/* Initlizes the event dispatcher;
Takes a char that when pressed terminates this proccess*/
void initDispatcher(char terminatingCharacter) {
#ifdef DEBUG
  file = fopen("eventLog.txt", "w");
  setbuf(file, NULL); // delete
#endif
  tc = terminatingCharacter;

  // event queue
  kpq = initQueue(initialEventCapacity);
  if (kpq == NULL) {
    exit(1);
  }

  // Puts in raw mode
  makeRaw();

  pthread_create(&pt, NULL, enqueueEvents, NULL);
  return;
}

/*Sets the line displine to raw*/
void makeRaw() {
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  cfmakeraw(&newt);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

/*'listens'/reads keypresses that are written to stdin
   enqueues these presses as an event
*/
void *enqueueEvents(void *arg) {
  event eventToEnque; // This is the event to be enequed
  char c;

  // Every iteration we read in from stdin 1 byte
  //  we then proccess it and workout what kind of key press it was; then
  //  enqueded we look until the tc is pressed
  while (c != tc) {
    read(STDIN_FILENO, &c, 1);
#ifdef DEBUG
    fprintf(file, "%c", c);
#endif

    // user hit the terminating character; end this thread
    if (c == tc) {
      eventToEnque.eventType = QUIT_SEQUENCE;
      kpq->en_stat = EINVALID;
      enqueue(kpq, eventToEnque);
      kpq->en_stat = EVALID;
      continue;
    }

    switch (c) {
    case '\033': // Control escape sequences; IE arrows
      eventToEnque.eventType = escapeChar();
      break;
    case 127: // Back space
      eventToEnque.eventType = BACKSPACE;
      break;
    case '\r': // Enter key was pressed
      eventToEnque.eventType = ENTER;
      kpq->en_stat = EINVALID;
      enqueue(kpq, eventToEnque);
      kpq->en_stat = EVALID;
      return NULL;
    //  eventToEnque.eventType = ENTER;
      break;
    default:
      eventToEnque.eventType = NON_CONTROL;
      eventToEnque.kp = c; // save the key that was pressed
      break;
    }

    if (kpq->de_stat == DINVALID)
      continue; // cant enqueue right now;

    kpq->en_stat = EINVALID; // To prevent race conditions; main thread may not
                             // deque while invalid
    // now enque the new keypress event
    enqueue(kpq, eventToEnque);
    kpq->en_stat = EVALID;
  };

  return NULL;
}

/*takes a pointer to an event and makes it equal to the most recent
  event; Returns 0 if no events are queued
*/
int pollEvent(event *event) {
  // No events queued right now or invalid
  if (kpq->size == 0 || kpq->en_stat == EINVALID) {
    event = NULL;
    return 0;
  }

  kpq->de_stat = DINVALID;
  *event = dequeue(kpq);
  kpq->de_stat = DVALID;

  return 1;
}

/*Terminates the dispatcher*/
void terminateDispatcher() {
  pthread_join(pt, NULL);
  destroyQueue(kpq);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return;
}

enum eventType escapeChar() {
  char c;

  read(STDIN_FILENO, &c, 1); // To get rid of the '['
  read(STDIN_FILENO, &c, 1); // Now we can read in which key was pressed

  switch (c) {
  case 'C': // Right arrow
    return RIGHT_ARROW;
    break;
  case 'D': // Left Arrow
    return LEFT_ARROW;
    break;
  case 'A': // Up Arrow
    return UP_ARROW;
    break;
  case 'B': // Down Arrow
    return DOWN_ARROW;
    break;
  case '3':                    // Delete
    read(STDIN_FILENO, &c, 1); // bc fully seq is \33[3~
    return DELETE;
    break;
  default:
    break;
  }
}