#include <stdio.h>
#include <unistd.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int distance = -1;
    
    int echoPin = GPIO_PIN1;

    if (Hcsr04Init(echoPin) == -1) {
        printf("Fail to init hcsr04\n");
    }
//while(1) {
    if (Hcsr04Read(&distance)==0 && distance != -1) {
        printf("Get distance: %3d cm\n", distance);
    } else {
        printf("Faid to get distance\n");
    }
//	usleep(1000*1000);
//}
    Hcsr04DeInit();
    return 0;
}
