#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

static struct sensor brSwitch[] = {
        {
                GPIO_PIN(7),
                IRQ_TYPE_EDGE_FALLING,
        }
};

int main(int argc, char ** argv)
{
    int i;
    int retSize = -1;
    char value[ARRAY_SIZE(brSwitch)];
    int devFD = -1;

    if (argc == 2) {
        brSwitch[0].pin = atoi(argv[1]);
    }
    
    printf("Using GPIO_PIN(%d)\n", brSwitch[0].pin);
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
    return 0;
}
