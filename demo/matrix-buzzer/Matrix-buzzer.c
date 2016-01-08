#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libfahw.h"

int parseCmd(int argc, char **argv, int *pin, int *Hz, int *duty)
{
    int num = atoi(argv[1]);
    int hz = atoi(argv[2]);
    int dt = atoi(argv[3]);
    
    if (hz<0) {
        return -1;
    }
    if (dt<0 || dt>1000) {
        return -1;
    }
    switch(num) {
    case 0:
        *pin = PWM0; 
        break;
    case 1:
        *pin = PWM1;
        break;
    case 2:
        *pin = PWM2;
        break;    
    default:
        printf("Unsupported pin PWM%d\n", num);
        num = 0;
        *pin = PWM0;
    }
    *Hz = hz;
    *duty = dt;
    printf("Using config: pin=PWM%d freq=%dHz duty=%d\n", num, hz, dt);
    return 0;
}

int main(int argc, char ** argv)
{
    int pin = PWM0;
    int Hz;
    int duty;
    
    if (argc == 4) {
        if (parseCmd(argc, argv, &pin, &Hz, &duty) == -1) {
            return -1;
        }
    } else {
        Hz = 1000;
        duty = 500;
        printf("Usage:%s PWM[0~2] freq duty[0~1000]\n", argv[0]);
        printf("Using default config: pin=PWM0 freq=%dHz duty=%d\n", Hz, duty);
    }
    
    if (PWMPlay(pin, Hz, duty) == -1) {
        printf("Fail to output PWM\n");
        return -1;
    }  
    printf("Press enter to stop PWM\n");
    getchar();
    PWMStop(pin);
    printf("Stopped PWM\n");
    return 0;
}
