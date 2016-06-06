#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "libfahw.h"

#define DRIVER_MODULE "matrix_pwm"
static int pwm;

void intHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        PWMStop(pwm);
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
    int Hz, duty, board;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    } 
    
    system("modprobe "DRIVER_MODULE);
    signal(SIGINT, intHandler);
    if (argc == 4) {
        // Usage:matrix-pwm channel freq duty[0~1000]
        pwm = atoi(argv[1]);
        Hz = atoi(argv[2]);
        duty = atoi(argv[3]);
    } else {
        Hz = 1000;
        duty = 500;
        printf("Using default config: channel=%d freq=%dHz duty=%d\n", pwm, Hz, duty);
    }
    if (PWMPlay(pwm, Hz, duty) == -1) {
        printf("Fail to output PWM\n");
    }  
    printf("Press enter to stop PWM\n");
    getchar();
    PWMStop(pwm);
    system("rmmod "DRIVER_MODULE);
    
    return 0;
}
