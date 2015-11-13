#ifndef __FRIENDLYARM_HARDWARE_GPIO_H__
#define __FRIENDLYARM_HARDWARE_GPIO_H__

// Direction
#define GPIO_IN                 (1) 
#define GPIO_OUT                (2) 

// Value
#define GPIO_LOW                (0)
#define GPIO_HIGH               (1)

//nanopi
#define GPIO_PIN1  (32 + 28) // GPIOB28
#define GPIO_PIN2  (32 + 29) // GPIOB29
#define GPIO_PIN3  (32 + 26) // GPIOB26
#define GPIO_PIN4  (32 + 30) // GPIOB30
#define GPIO_PIN5  (32 + 31) // GPIOB31
#define GPIO_PIN6  (32 + 27) // GPIOB27
#define GPIO_PIN7  (64 + 8)  // GPIOC8
#define GPIO_PIN8  (64 + 28) // GPIOC28
#define GPIO_PIN9  (64 + 7)  // GPIOC7
#define GPIO_PIN10 (160 + 2) // ALIVEGPIO2
#define GPIO_PIN11 (160 + 3) // ALIVEGPIO3

#define PWM0       (96 + 1)  // GPIOD1
#define PWM1       (64 + 13) // GPIOC13
#define PWM2       (64 + 14) // GPIOC14

int exportGPIOPin(int pin);
int unexportGPIOPin(int pin);

// GPIO_LOW or GPIO_HIGH
int setGPIOValue(int pin, int value);
int getGPIOValue(int pin);

// GPIO_IN or GPIO_OUT
int setGPIODirection(int pin, int direction);
int getGPIODirection(int pin);

#endif
