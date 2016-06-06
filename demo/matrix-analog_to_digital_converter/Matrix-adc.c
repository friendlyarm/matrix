#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "libfahw.h"

#define BUF_SIZE    (32)
#define ADC_READ_TIMES (1)
#define DRIVER_MODULE "pcf8591"

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
    int value = 0;
    int channel = 0;

    if (boardInit() < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    if (argc == 2)
        channel = atoi(argv[1]);
    system("modprobe "DRIVER_MODULE);
    signal(SIGINT, intHandler);
    for (i=0; i<ADC_READ_TIMES; i++) {
        if (pcf8591Read(channel, &value) != -1) {
            printf("The channel%d value is %d\n", channel, value);
        } else {
            printf("Fail to get channel%d value\n", channel);
        }
    }
    system("rmmod "DRIVER_MODULE);
    
    return 0;
}
