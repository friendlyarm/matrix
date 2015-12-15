#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

static struct sensor sound[] = {
        {
                GPIO_PIN(7),
                IRQ_TYPE_EDGE_BOTH,
        }
};

int main(int argc, char ** argv)
{
    int i;
    int retSize = -1;
    char value[ARRAY_SIZE(sound)];
    int devFD = -1;

    if (argc == 2) {
        sound[0].pin = atoi(argv[1]);
    }
    
    printf("Using GPIO_PIN(%d)\n", sound[0].pin);
    if ((devFD =sensorInit(sound, ARRAY_SIZE(sound))) == -1) {
        printf("Fail to init sensor\n");
        return -1;
    }
    printf("Please speak...\n");
    if ((retSize = sensorRead(devFD, value, ARRAY_SIZE(sound))) == -1) {
        printf("Fail to read sensors\n");
    }
    if (retSize > 0) {
        i = 0;
        for(i=0; i<retSize; i++)
        {
            printf("sound[%d]:%d\n", i, value[i]);
        }
    }
    sensorDeinit(devFD);
    return 0;
}
