#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "libfahw.h"

#define BUF_SIZE    (32)
#define ADC_READ_TIMES (100)
#define DRIVER_MODULE "pcf8591"

void intHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Quit reading\n");
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
    int i = 0;
    int value = 0;
    int channel = 0;

    if (argc == 2)
        channel = atoi(argv[1]);
    if (boardInit() < 0)
        printf("Fail to init board\n");
    
    system("modprobe "DRIVER_MODULE);
    signal(SIGINT, intHandler);
    for (i=0; i<ADC_READ_TIMES; i++) {
        if (pcf8591Read(channel, &value) != -1) {
            printf("channel%d value=%d\n", channel, value);
        } else {
            printf("Fail to get channel%d value\n", channel);
            return -1;
        }
        sleep(1);
    }
    system("rmmod "DRIVER_MODULE);
    return 0;
}
