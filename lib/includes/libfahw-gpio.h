#ifndef __FRIENDLYARM_HARDWARE_GPIO_H__
#define __FRIENDLYARM_HARDWARE_GPIO_H__

// Direction
#define GPIO_IN                 (1) 
#define GPIO_OUT                (2) 

// Value
#define GPIO_LOW                (0)
#define GPIO_HIGH               (1)

// Tiny4412
#define GPIO_PIN1      (219)
#define GPIO_PIN2      (220) 
#define GPIO_PIN3      (221) 
#define GPIO_PIN4      (222)
#define GPIO_PIN5      (20)
#define GPIO_PIN6      (21)
#define GPIO_PIN7      (22)
#define GPIO_PIN8      (23) 
#define GPIO_PIN9      (199) 
#define GPIO_PIN10     (206)
#define PWM_PIN1           (37)
#define PWM_PIN2           (38)


int exportGPIOPin(int pin);
int unexportGPIOPin(int pin);

// GPIO_LOW or GPIO_HIGH
int setGPIOValue(int pin, int value);
int getGPIOValue(int pin);

// GPIO_IN or GPIO_OUT
int setGPIODirection(int pin, int direction);
int getGPIODirection(int pin);

#endif
