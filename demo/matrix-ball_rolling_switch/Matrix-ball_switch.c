#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libfahw.h"

#define DRIVER_MODULE "matrix_gpio_int"

static struct sensor brSwitch[] = {
        {
                GPIO_PIN(7),
                IRQ_TYPE_EDGE_FALLING,
        }
};
int devFD = -1;

void intHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Quit reading\n");
        sensorDeinit(devFD);
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
    int i, board;
    int retSize = -1;
    char value[ARRAY_SIZE(brSwitch)];

    if (argc == 2) {
        brSwitch[0].pin = atoi(argv[1]);
    }
    
    printf("Using GPIO_PIN(%d)\n", brSwitch[0].pin);
    if ((board = boardInit()) < 0)
        printf("Fail to init board\n");
    system("modprobe "DRIVER_MODULE);
    signal(SIGINT, intHandler);
    if (board == BOARD_NANOPI_T2)
        brSwitch[0].pin = GPIO_PIN(15);
    if ((devFD =sensorInit(brSwitch, ARRAY_SIZE(brSwitch))) == -1) {
        printf("Fail to init sensor\n");
        return -1;
    }
    printf("Lean the switch...\n");
    if ((retSize = sensorRead(devFD, value, ARRAY_SIZE(brSwitch))) == -1) {
        printf("Fail to read sensors\n");
    }
    if (retSize > 0) {
        i = 0;
        for (i=0; i<retSize; i++) {
            printf("Switch[%d]:%d\n", i, value[i]);
        }
    }
    sensorDeinit(devFD);
    system("rmmod "DRIVER_MODULE);
    return 0;
}
