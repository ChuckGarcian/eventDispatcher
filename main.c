#include <stdio.h>
#include <stdlib.h>

#include "KeyPressEventDispatcher.h"
#include "queue.h"
#include <unistd.h>

int main (void) {
    initDispatcher('q');
    event event; // struct that contains key event info
    while(1) {
        if (pollEvent(&event) == 1) {
            char c = event.kp; // char that was pressed
            switch (event.eventType)
            {
            case QUIT_SEQUENCE:
                terminateDispatcher();
                exit(1);
                break;
            case NON_CONTROL:
                printf("%c", c);
                write(STDOUT_FILENO, &c, 1);
                break;
            }
        }               
    }    

}