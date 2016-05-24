#ifndef __FRIENDLYARM_HARDWARE_GPIO_H__
#define __FRIENDLYARM_HARDWARE_GPIO_H__

// Direction
#define GPIO_IN                 (1) 
#define GPIO_OUT                (2) 

// Value
#define GPIO_LOW                (0)
#define GPIO_HIGH               (1)

//nanopi
#define GPIO_PIN(x)             (x)

#define PWM0       (96 + 1)  // GPIOD1
#define PWM1       (64 + 13) // GPIOC13
#define PWM2       (64 + 14) // GPIOC14

#define GPIO_FILENAME_DEFINE(pin,field) char fileName[255] = {0}; \
        sprintf(fileName, "/sys/class/gpio/gpio%d/%s", pin, field);

int initPinGPIO(int board);
int pintoGPIO(int pin);
int exportGPIOPin(int pin);
int unexportGPIOPin(int pin);

// GPIO_LOW or GPIO_HIGH
int setGPIOValue(int pin, int value);
int getGPIOValue(int pin);

// GPIO_IN or GPIO_OUT
int setGPIODirection(int pin, int direction);
int getGPIODirection(int pin);

#endif
