#include <stdio.h>
#include "libfahw.h"

static struct sensor button[] = {
        {
                GPIO_PIN1,
                IRQ_TYPE_EDGE_FALLING,
        }
};

int main(void)
{
    int i;
    int retSize = -1;
    char value[ARRAY_SIZE(button)];
    int devFD = -1;
    if ((devFD =sensorInit(button, ARRAY_SIZE(button))) == -1) {
        printf("Fail to init sensor\n");
        return -1;
    }
    printf("Press the button...\n");
    if (( retSize = sensorRead(devFD, value, ARRAY_SIZE(button)) ) == -1) {
        printf("Fail to read sensors\n");
    }
    if (retSize > 0) {
        i = 0;
        for (i=0; i<retSize; i++) {
            printf("Button[%d]:%d\n", i, value[i]);
        }
    }
    sensorDeinit(devFD);
    return 0;
}
