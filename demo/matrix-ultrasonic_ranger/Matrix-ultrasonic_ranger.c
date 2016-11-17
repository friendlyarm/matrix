#include <stdio.h>
#include <unistd.h>
#include "libfahw.h"

#define DRIVER_MODULE       "matrix_hcsr04"

int main(int argc, char ** argv)
{
    int distance = -1;
    int pin = GPIO_PIN(7);
    int board;

    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    if (board == BOARD_NANOPC_T2 || board == BOARD_NANOPC_T3)
        pin = GPIO_PIN(15);
    
    system("modprobe "DRIVER_MODULE);
    if (Hcsr04Init(pin) == -1) {
        printf("Fail to init hcsr04\n");
        goto err;
    }
    if (Hcsr04Read(&distance) != -1) {
        printf("The distance is %3d cm\n", distance);
    } else {
        printf("Faid to get distance\n");
    }
    Hcsr04DeInit();
err:
    system("rmmod "DRIVER_MODULE);
    
    return 0;
}
