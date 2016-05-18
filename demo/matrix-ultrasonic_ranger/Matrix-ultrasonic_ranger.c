#include <stdio.h>
#include <unistd.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int distance = -1;
    int pin = GPIO_PIN(7);

    boardInit();
    if (Hcsr04Init(pin) == -1) {
        printf("Fail to init hcsr04\n");
    }
    if (Hcsr04Read(&distance) != -1) {
        printf("Get distance: %3d cm\n", distance);
    } else {
        printf("Faid to get distance\n");
    }
    Hcsr04DeInit();
    return 0;
}
