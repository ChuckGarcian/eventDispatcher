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

#define initialEventCapacity 200

kpQueue *kpq;
char tc;                   // terminating character
struct termios newt, oldt; // to change/save line displcine
pthread_t pt; // event listener thread

enum eventType escapeChar();

/* Initlizes the event dispatcher;
Takes a char that when pressed terminates this proccess*/
void initDispatcher(char terminatingCharacter) {
  tc = terminatingCharacter;

  // Puts in raw mode
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  cfmakeraw(&newt);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // event queue
  kpq = initQueue(initialEventCapacity);
  if (kpq == NULL) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    exit(1);
  }

  pthread_create(&pt, NULL, enqueueEvents, NULL);
  return;
}

FILE *file;
/*'listens'/reads keypresses that are written to stdin
   enqueues these presses as an event
*/
void * enqueueEvents(void *arg) {
    event eventToEnque; // This is the event to be enequed
    file = fopen("evenLog.txt", "w");
    setbuf(file, NULL); // delete

    char c;

    while (1) {
        if (kpq->de_stat == INVALID) continue;
        read(STDIN_FILENO, &c, 1);
        fprintf(file, "%c", c);

        if (c == tc) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return NULL;
        }

        switch (c) {
            case '\033': // Control escape sequences; IE arrows
                eventToEnque.eventType = escapeChar();
                break;
            case 127: // Back space
                eventToEnque.eventType = BACKSPACE;
                break;
            default:
                eventToEnque.eventType = NON_CONTROL;
                eventToEnque.kp = c; // save the key that was pressed
                break;
        }

        kpq->en_stat = INVALID; //To prevent race conditions; main thread may not deque while invalid

        // now enque the new keypress event
        enqueue(kpq, eventToEnque);

        kpq->en_stat = VALID;
    };
}

/*takes a pointer to an event and makes it equal to the most recent
  event; Returns 0 if no events are queued
*/
int pollEvent(event *event) {
    // No events queued right now or invalid
    if (kpq->size == 0 || kpq->en_stat == INVALID) { 
        event = NULL;
        return 0;
    }

    *event = dequeue(kpq);
    return 1;
}

/*Terminates the dispatcher*/
void terminateDispatcher() {
  destroyQueue(kpq);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  exit(0);
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
        default:
            break;
    }
}