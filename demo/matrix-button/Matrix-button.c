#include <stdio.h>
#include "libfahw.h"

static struct sensor button[] = {
        {
                GPIO_PIN1,
                IRQ_TYPE_EDGE_FALLING,
        }
};

void parseCmd(int argc, char **argv, int *pin)
{
    int num = atoi(argv[1]);
    switch(num) {
    case 1:
        *pin = GPIO_PIN1; 
        break;
    case 2:
        *pin = GPIO_PIN2;
        break;
    case 3:
        *pin = GPIO_PIN3;
        break;
    case 4:
        *pin = GPIO_PIN4;
        break;
    case 5:
        *pin = GPIO_PIN5;
        break;
    case 6:
        *pin = GPIO_PIN6;
        break;
    case 7:
        *pin = GPIO_PIN7;
        break;
    case 8:
        *pin = GPIO_PIN8;
        break;
    case 9:
        *pin = GPIO_PIN9;
        break;
    case 10:
        *pin = GPIO_PIN10;
        break;
    case 11:
        *pin = GPIO_PIN11;
        break;    
    default:
        printf("Unsupported pin GPIO_PIN%d\n", num);
        num = 1;
        *pin = GPIO_PIN1;
    }
    printf("Using pin GPIO_PIN%d\n", num);    
}

int main(int argc, char ** argv)
{
    int i;
    int retSize = -1;
    char value[ARRAY_SIZE(button)];
    int devFD = -1;
    int pin = -1;

    if (argc == 2) {
        parseCmd(argc, argv, &pin);
        button[0].gpio = pin;
    } else {
        printf("Using default pin GPIO_PIN1\n");
    }

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
