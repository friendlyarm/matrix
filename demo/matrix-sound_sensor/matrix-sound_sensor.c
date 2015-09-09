#include <stdio.h>
#include "libfahw.h"

static struct sensor sound[] = {
        {
                GPIO_PIN1,
                IRQ_TYPE_EDGE_BOTH,
        }
};

int main(void)
{
    int i;
    int retSize = -1;
    char value[ARRAY_SIZE(sound)];
    int devFD = -1;
    if ((devFD =sensorInit(sound, ARRAY_SIZE(sound))) == -1) {
        printf("Fail to init sensor\n");
        return -1;
    }

    if (( retSize = sensorRead(devFD, value, ARRAY_SIZE(sound)) ) == -1) {
        printf("Fail to read sensors\n");
    }
    if (retSize > 0) {
        i = 0;
        for(i=0; i<retSize; i++)
        {
            printf("sound[%d]:%d\n", i, value[i]);
        }
        printf("\n");
    }
    sensorDeinit(devFD);
    return 0;
}
