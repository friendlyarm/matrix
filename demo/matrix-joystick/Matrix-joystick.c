#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "libfahw.h"

#define PS2_READ_TIMES      (1000)
#define DRIVER_MODULE       "pcf8591"

void intHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
    int i = 0;
    int x, y, board;
    
    x = y = 0;
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    system("modprobe "DRIVER_MODULE);
    signal(SIGINT, intHandler);
    for (i=0; i<PS2_READ_TIMES; i++) {
        if (pcf8591Read(1, &x) != -1 && pcf8591Read(2, &y) != -1) {
            printf("x=%4d y=%4d\n", x, y);
        }
    }
    system("rmmod "DRIVER_MODULE);
    return 0;
}
