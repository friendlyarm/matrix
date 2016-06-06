#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/input.h>
#include "libfahw.h"

#define LED_BLINK_TIMES (2)
#define LED_NUM         (4)
int testLED(int board)
{
    int i, j, ret;
    int pin[LED_NUM]={22, 31, 37, 35};
    ret = 0;
    
    if (board == BOARD_NANOPI_2) 
        pin[0] = 33;
    for (i=0; i<LED_NUM; i++) {
        if ((ret = exportGPIOPin(pin[i])) == -1) {   
            printf("exportGPIOPin(%d) failed\n", pin[i]);
        }
        if ((ret = setGPIODirection(pin[i], GPIO_OUT)) == -1) {
            printf("setGPIODirection(%d) failed\n", pin[i]);
        }
    }
    for (i=0; i<LED_BLINK_TIMES; i++) {
        if (i % 2) {
            for (j=0; j<4; j++)
                ret = setGPIOValue(pin[j], GPIO_HIGH);
        } else {
            for (j=0; j<4; j++)
                ret = setGPIOValue(pin[j], GPIO_LOW);
        }
        printf("LED blinking %d\n", i+1);
        sleep(1);
    }
    for (i=0; i<LED_NUM; i++) {
        if ((ret = unexportGPIOPin(pin[i])) == -1) {   
            printf("unexportGPIOPin(%d) failed\n", pin[i]);
        }
    }
    
    return ret;
}

#define BUTTON_NUM      (3)
int readButton()
{
    int i, ret;
    int pin[3]={36, 38, 40};
    int value[3];
    
    ret = 0;
    printf("Button: ");
    for (i=0; i<BUTTON_NUM; i++) {
        if ((ret = exportGPIOPin(pin[i])) == -1) {   
            printf("exportGPIOPin(%d) failed\n", pin[i]);
        }
        if ((ret = setGPIODirection(pin[i], GPIO_IN)) == -1) {
            printf("setGPIODirection(%d) failed\n", pin[i]);
        }
        value[i] = getGPIOValue(pin[i]);
        if ((ret = unexportGPIOPin(pin[i])) == -1) {   
            printf("unexportGPIOPin(%d) failed\n", pin[i]);
        } else {
            printf("%2d", value[i]);
        }
    }
    printf("\n");
    
    return ret;
}

#define ADC_DRIVER_MODULE "pcf8591"
#define ADC_READ_TIMES (1)
int readADC()
{
    int i, value, channel;
    
    channel = 0;
    system("modprobe "ADC_DRIVER_MODULE);
    for (i=0; i<ADC_READ_TIMES; i++) {
        if (pcf8591Read(channel, &value) != -1) {
            printf("The channel%d value is %d\n", channel, value);
        } else {
            printf("Fail to get channel%d value\n", channel);
        }
    }
    system("rmmod "ADC_DRIVER_MODULE);
    
    return 0;
}

int readCompass()
{
    int devFD, i2cDev;
    double angle;
    
    i2cDev = 0;
    if ((devFD = hmc5883Init(i2cDev)) == -1) {
        printf("Fail to init hmc5883\n");
        return -1;
    }
    if ((angle = hmc5883Read(devFD)) != -1) {
        printf("The angle is %.1f\n", angle);
    } else {
        printf("Fail to read hmc5883\n");
    }
    hmc5883DeInit(devFD);
    
    return 0;
}

#define BUF_SIZE    (64)
#define TEMP_DRIVER_MODULE        "w1-gpio"
#define TEMP_GPIO_DRIVER_MODULE   "w1-gpio-board"
int readTemp()
{
    char temperature[BUF_SIZE];
    char modStr[BUF_SIZE];
    int pin = GPIO_PIN(29);
    
    system("modprobe "TEMP_DRIVER_MODULE);
    sprintf(modStr, "modprobe %s gpio=%d", TEMP_GPIO_DRIVER_MODULE, pintoGPIO(pin));
    system(modStr);
    sleep(1);
    memset(temperature, 0, BUF_SIZE);
    if (ds18b20Read(temperature) > 0) {
        printf("The temperature is %.3f C\n", atoi(temperature)/1000.0);
    } else {
        printf("Fail to get temperature\n");        
    }
    system("rmmod "TEMP_GPIO_DRIVER_MODULE);
    system("rmmod "TEMP_DRIVER_MODULE);
    
    return 0;
}

#define PWM_DRIVER_MODULE "matrix_pwm"
static int pwm;
void PWMIntHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        PWMStop(pwm);
        system("rmmod "PWM_DRIVER_MODULE);
    }
    exit(0);
}
int testPWM(int board)
{
    int Hz, duty;
    
    Hz = 1000;
    duty = 500;
    if (board == BOARD_NANOPI_M2 || board == BOARD_NANOPI_2_FIRE || board == BOARD_NANOPI_2)
        pwm = 0;
    else 
        return -1;
    system("modprobe "PWM_DRIVER_MODULE); 
    signal(SIGINT, PWMIntHandler);
    printf("Pwm start\n");
    if (PWMPlay(pwm, Hz, duty) == -1) {
        printf("Fail to output PWM\n");
    }
    sleep(1);
    PWMStop(pwm);
    system("rmmod "PWM_DRIVER_MODULE);
    printf("Pwm stop\n");
    
    return 0;
}

#define GPIO_IR_DEV         "/dev/input/gpio_ir_recv"
#define IR_EVENT_TIMES      (6)
#define IR_DRIVER_MODULE    "matrix_ir_recv"
static int irFD;
void IRIntHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        closeHW(irFD);
        system("rmmod "IR_DRIVER_MODULE);
    }
    exit(0);
}
int testIR()
{
    int i, j;
    int retSize = -1;
    char *devName = GPIO_IR_DEV;
    int pin = GPIO_PIN(32);
    char modStr[BUF_SIZE];
    struct input_event evKey;

    sprintf(modStr, "modprobe %s gpio=%d", IR_DRIVER_MODULE, pintoGPIO(pin));
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
    system("rmmod "IR_DRIVER_MODULE);
    
    return 0;
}

int main(int argc, char ** argv)
{
    int board;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    testLED(board);
    readButton();
    readADC();
    readCompass();
    testPWM(board);
    // readTemp();
    // testIR();
    
    return 0;
}
