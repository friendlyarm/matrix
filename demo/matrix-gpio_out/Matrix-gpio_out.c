#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfahw.h"

#define STATUS_CHANGE_TIMES     (5)

int main(int argc, char ** argv) 
{
    int pin = GPIO_PIN(7); 
    int i, value, board;
    int ret = -1;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    if (board == BOARD_NANOPC_T2 || board == BOARD_NANOPC_T3)
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
            value = GPIO_HIGH;
        } else {
            value = GPIO_LOW;
        }
        if ((ret = setGPIOValue(pin, value)) > 0) {
            printf("%d: GPIO_PIN(%d) value is %d\n", i+1, pin, value);
        } else {
            printf("setGPIOValue(%d) failed\n", pin);
        }
        sleep(1);
    }
    unexportGPIOPin(pin);
    return 0;
}
