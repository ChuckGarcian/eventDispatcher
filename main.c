#include <stdio.h>
#include <stdlib.h>

#include "KeyPressEventDispatcher.h"
#include "queue.h"
#include <unistd.h>

int main (void) {
    printf("Starting!!\n");
    initDispatcher('q');
      

    printf("ENDING\n");

    // terminateDispatcher();
    event event;

    while(1) {
        if (pollEvent(&event) == 1) {
            char c = event.kp;
            write(STDOUT_FILENO, &c, 1);
        }               
    }    

}