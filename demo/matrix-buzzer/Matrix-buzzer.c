#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int pwm, Hz, duty;
    int board;
    
    board = boardInit();
    switch(board) {
    case BOARD_NANOPI_M1:
        pwm = 1;
        break;
    default:
        return -1;
    }
    
    if (argc == 4) {
        pwm = atoi(argv[1]);
        Hz = atoi(argv[2]);
        duty = atoi(argv[3]);
    } else {
        Hz = 1000;
        duty = 500;
        printf("Usage:%s channel freq duty[0~1000]\n", argv[0]);
        printf("Using default config: channel=%d freq=%dHz duty=%d\n", pwm, Hz, duty);
    }
    
    if (PWMPlay(pwm, Hz, duty) == -1) {
        printf("Fail to output PWM\n");
        return -1;
    }  
    printf("Press enter to stop PWM\n");
    getchar();
    PWMStop(pwm);
    printf("Stopped PWM\n");
    return 0;
}
