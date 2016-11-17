#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/input.h>
#include "libfahw.h"

#define BUF_SIZE            (64)
#define GPIO_IR_DEV         "/dev/input/gpio_ir_recv"
#define IR_EVENT_TIMES      (6)
#define DRIVER_MODULE       "matrix_ir_recv"
static int irFD;
void IRIntHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        closeHW(irFD);
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
    int board, i, j;
    int retSize = -1;
    char *devName = GPIO_IR_DEV;
    int pin = GPIO_PIN(7);
    char modStr[BUF_SIZE];
    struct input_event evKey;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    if (board == BOARD_NANOPC_T2 || board == BOARD_NANOPC_T3)
        pin = GPIO_PIN(15);
    
    sprintf(modStr, "modprobe %s gpio=%d", DRIVER_MODULE, pintoGPIO(pin));
    system(modStr);
    signal(SIGINT, IRIntHandler);
    sleep(1);
    irFD = openHW(devName, O_RDWR);
    if (irFD < 0) {
        printf("Fail to open GPIO IR device\n");
        goto err;
    }
    printf("Press the IR remoter\n");
    for (i=0; i<IR_EVENT_TIMES; i++) {
        if (selectHW(irFD, 0, 0) == 1) {
            retSize = readHW(irFD, &evKey, sizeof(struct input_event));
            for (j=0; j<(int) retSize / sizeof(struct input_event); j++)
                printf("%2d: Type=%d, Code=%d, Value=%x\n", i, evKey.type, evKey.code, evKey.value);
        }
    }
    closeHW(irFD);
err:
    system("rmmod "DRIVER_MODULE);
    return 0;
}
