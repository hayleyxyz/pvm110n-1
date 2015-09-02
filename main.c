#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <k8055.h>

//#include "cstring.h"

int main(int argc, char **argv) {

    int ipid = 0;
    WINDOW *mainWin;
    int i;
    long digitalInput = 0;
    long analogInputs[2];


    // Open USB device
    if(OpenDevice(ipid) < 0) {
        printf("Could not open the k8055 (port:%d)\nPlease ensure that the device is correctly connected.\n", ipid);
        return EXIT_FAILURE;
    }

    // Init ncurses
    mainWin = initscr();
    if(mainWin == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        return EXIT_FAILURE;
    }

    // Hide cursor
    curs_set(0);

    for(;;) {
        // Clear window
        clear();

        // Digital inputs
        digitalInput = ReadAllDigital();

        if(digitalInput == K8055_ERROR) {
            printw("ReadAllDigital() ERROR\n");
        }
        else {
            // Heading
            printw("- Digital inputs\n");

            // Digital inputs
            for (i = 0; i < 5; i++) {
                printw("\t%d: %s\n", (i + 1), (digitalInput & (1 << i)) ? "on" : "off");
            }
        }

        if(ReadAllAnalog(&analogInputs[0], &analogInputs[1]) == K8055_ERROR) {
            printw("ReadAllAnalog() ERROR\n");
        }
        else {
            printw("Analog 1: 0x%08x\n", analogInputs[0]);
            printw("Analog 2: 0x%08x\n", analogInputs[1]);
        }

        OutputAnalogChannel(1, 0xff);

        refresh();
        usleep(1000); // 1 millisecond
    }

    return EXIT_SUCCESS;
}