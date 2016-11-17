#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libfahw.h"

#define DRIVER_MODULE "matrix_gpio_int"

static struct sensor dev[] = {
        {
                GPIO_PIN(7),
                IRQ_TYPE_EDGE_FALLING,
        }
};
int devFD = -1;

void intHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        sensorDeinit(devFD);
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
    int i, board;
    int retSize = -1;
    char value[ARRAY_SIZE(dev)];

    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    if (argc == 2)
        dev[0].pin = atoi(argv[1]);
    system("modprobe "DRIVER_MODULE);
    signal(SIGINT, intHandler);
    if (board == BOARD_NANOPC_T2 || board == BOARD_NANOPC_T3)
        dev[0].pin = GPIO_PIN(15);
    printf("Use GPIO_PIN(%d)\n", dev[0].pin);
    if ((devFD =sensorInit(dev, ARRAY_SIZE(dev))) == -1) {
        printf("Fail to init sensor\n");
        goto err;
    }
    printf("Waiting event...\n");
    if ((retSize = sensorRead(devFD, value, ARRAY_SIZE(dev))) == -1) {
        printf("Fail to read sensors\n");
    }
    if (retSize > 0) {
        i = 0;
        for (i=0; i<retSize; i++) {
            printf("Device[%d] value is %d\n", i, value[i]);
        }
    }
    sensorDeinit(devFD);
err:    
    system("rmmod "DRIVER_MODULE);
    return 0;
}
