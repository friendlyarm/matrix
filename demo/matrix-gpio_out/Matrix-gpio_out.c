#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfahw.h"

#define STATUS_CHANGE_TIMES     (5)

int main(int argc, char ** argv) 
{
    int pin = GPIO_PIN(7); 
    int i, board;
    int ret = -1;
    
    if ((board = boardInit()) < 0)
        printf("Fail to init board\n");
    if (board == BOARD_NANOPI_T2)
        pin = GPIO_PIN(15);
    if (argc == 2)
        pin = GPIO_PIN(atoi(argv[1]));
    
    if ((ret = exportGPIOPin(pin)) == -1) {   
        printf("exportGPIOPin(%d) failed\n", pin);
    }
    if ((ret = setGPIODirection(pin, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", pin);
    }

    for (i = 0; i < STATUS_CHANGE_TIMES; i++) {
        if (i % 2) {
            ret = setGPIOValue(pin, GPIO_HIGH);
        } else {
            ret = setGPIOValue(pin, GPIO_LOW);
        }
        if (ret == -1) {
            printf("setGPIOValue(%d) failed\n", pin);
        }
        printf("%d: gpio status change\n", i+1);
        sleep(1);
    }
    unexportGPIOPin(pin);
    return 0;
}
