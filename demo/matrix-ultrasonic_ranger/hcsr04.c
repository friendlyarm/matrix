#include <stdio.h>
#include <unistd.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int distance = -1;
    
    int echoPin = GPIO_PIN1;
    int triggerPin = GPIO_PIN2;
    
    if (Hcsr04Init(echoPin, triggerPin) == -1) {
        printf("Fail to init hcsr04\n");
    }
    
    if (Hcsr04Read(&distance)==0 && distance != -1) {
        printf("Get distance: %3d cm\n", distance);
    } else {
        printf("Faid to get distance\n");
    }
    Hcsr04DeInit();
    return 0;
}
