#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfahw.h"

#define LED_BLINK_TIMES     (5)

int main(int argc, char ** argv) 
{
    int ledPin = GPIO_PIN(7); 
    int i = 0;
    int ret = -1;

    if (argc == 2) {
        ledPin = GPIO_PIN(atoi(argv[1]));
    }
    
    boardInit();
    if ((ret = exportGPIOPin(ledPin)) == -1) {   
        printf("exportGPIOPin(%d) failed\n", ledPin);
    }
    if ((ret = setGPIODirection(ledPin, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", ledPin);
    }

    for (i = 0; i < LED_BLINK_TIMES; i++) {
        if (i % 2) {
            ret = setGPIOValue(ledPin, GPIO_HIGH);
        } else {
            ret = setGPIOValue(ledPin, GPIO_LOW);
        }
        if (ret == -1) {
            printf("setGPIOValue(%d) failed\n", ledPin);
        }
        printf("LED blinking times %d\n", i+1);
        sleep(1);
    }
    unexportGPIOPin(ledPin);
    return 0;
}
