
// Possible Keypress events
enum evenType{
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
    enum evenType evenType;
    char * kp; // contains Non control char pressed; NULL otherwise
} event;

/* Initlizes the event dispatcher*/
void initDispatcher();

/*'listens'/reads keypresses that are written to stdin
   enqueues these presses as an event
*/
void * enqueueEvents(void * arg);

/*returns the next event*/
int pollEvent(event * event);

/*Terminates the dispatcher*/
void terminateDispatcher();