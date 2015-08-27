#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfahw.h"

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
    default:
        printf("Unsupported pin GPIO_PIN%d\n", num);
        num = 1;
        *pin = GPIO_PIN1;
    }
    printf("Using pin GPIO_PIN%d\n", num);    
}

int main(int argc, char ** argv)
{
    int devFD = -1;
    int pin = GPIO_PIN1;
    char *temperature = (char *) malloc(32);
    memset(temperature, 0, 32);
    
    if (argc == 2) {
        parseCmd(argc, argv, &pin);
    } else {
        printf("Using default pin GPIO_PIN1\n");
    }

    if ((devFD = ds18b20Init(pin)) == -1) {
        printf("Fail to init ds18b20\n");
        return -1;
    }
    if (ds18b20Read(temperature) > 0) {
        printf("Temperature = %s\n", temperature);
    } else {
        printf("Fail to get temperature\n");		
    }
    free(temperature);
    ds18b20DeInit(devFD);
    return 0;
}
